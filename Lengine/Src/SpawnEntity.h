#pragma once

#include <lecs.h>
#include <cstdint>

#include "Components/Components.h"
#include "Game.h"

extern std::atomic<Game*> game;

// to do: add data file to directly load properties of components of entities

namespace spawn
{
	inline lecs::Entity* StaticObject(Vector2Df position, int tile_size, std::string texture_id)
	{
		lecs::Entity* static_obj = &game.load()->ecs_managers.entity_manager->AddEntity();
		SpriteComponent* sprite = static_obj->AddComponent<SpriteComponent>(
			texture_id,
			Vector2Df(0.f, game.load()->texture_manager.GetTexture(texture_id).getSize().y - tile_size)
			);
		TransformComponent* transform = static_obj->AddComponent<TransformComponent>(
			position,
			sprite->sprite.getTextureRect().width,
			sprite->sprite.getTextureRect().height,
			game.load()->world_scale);
		ColliderComponent* collider = static_obj->AddComponent<ColliderComponent>(
			position + Vector2Df(16.f, 16.f) * game.load()->world_scale,
			32 * game.load()->world_scale.x, 32 * game.load()->world_scale.y,
			false,
			ColliderComponent::TAG::STATIC
			);
		ProjColComponent* projcol = static_obj->AddComponent<ProjColComponent>(
			position + Vector2Df(16.f, 8.f) * game.load()->world_scale,
			34 * game.load()->world_scale.x, 34 * game.load()->world_scale.y,
			false
			);

		game.load()->ecs_managers.entity_manager->AddToGroup(static_obj, lecs::GRP_ENTITY);

		return static_obj;
	}

	inline lecs::Entity* Staircase(Vector2Df position, std::string texture_id)
	{
		lecs::Entity* staircase = &game.load()->ecs_managers.entity_manager->AddEntity();
		SpriteComponent* sprite = staircase->AddComponent<SpriteComponent>(
			texture_id,
			Vector2Df(0.f, 0.f)
			);
		TransformComponent* transform = staircase->AddComponent<TransformComponent>(
			position,
			sprite->sprite.getTextureRect().width,
			sprite->sprite.getTextureRect().height,
			game.load()->world_scale);

		game.load()->ecs_managers.entity_manager->AddToGroup(staircase, lecs::GRP_BACKGROUND_ENTITY);

		return staircase;
	}

	inline lecs::Entity* Map()
	{
		int map_size = 50;
		int floor_i = 0;
		int wall_i = 2;
		int side_wall_i = 1;
		int tile_size = 32;

		lecs::Entity* map = &game.load()->ecs_managers.entity_manager->AddEntity();
		TileMapComponent* tilemap = map->AddComponent<TileMapComponent>("terrain", tile_size, game.load()->world_scale.x);
		LevelComponent* level = map->AddComponent<LevelComponent>(-1);
		level->GenMap(8, map_size, 5, 10, floor_i, wall_i, side_wall_i);
		tilemap->top_layer_i = 2;
		tilemap->LoadMap(level->map);
		level->SetStaticTextures({ "crate", "d_crate" });
		level->GenStatics(5, 8, 32, game.load()->ecs_managers.entity_manager);
		level->GenStair(32);
		BoundaryComponent* boundary = map->AddComponent<BoundaryComponent>(level->map, wall_i, side_wall_i, map_size, map_size, tile_size * game.load()->world_scale.x, Vector2Df(0.f ,0.f) * game.load()->world_scale);

		game.load()->ecs_managers.entity_manager->AddToGroup(map, lecs::GRP_TERRAIN);

		return map;
	}

	inline lecs::Entity* Weapon(Vector2Df pos, bool isDropped)
	{
		lecs::Entity* item = &game.load()->ecs_managers.entity_manager->AddEntity();
		SpriteComponent* sprite = item->AddComponent<SpriteComponent>("glock", Vector2Df(4.5f, 1.f));
		sprite->SetDrawOrderPoint(isDropped ? 0.f : BaseItemComponent::equipped_dop * game.load()->world_scale.y);
		TransformComponent* transform = item->AddComponent<TransformComponent>(pos, sprite->sprite.getTextureRect().width,
			sprite->sprite.getTextureRect().height, game.load()->world_scale);
		GunComponent* gun = item->AddComponent<GunComponent>(
			Vector2Df
			(
				sprite->sprite.getTextureRect().width * game.load()->world_scale.x,
				0.f
			),
			75.f);
		BaseItemComponent* bitem = item->AddComponent<BaseItemComponent>(isDropped);

		game.load()->ecs_managers.entity_manager->AddToGroup(item, lecs::GRP_ENTITY);

		return item;
	}

	inline lecs::Entity* Projectile(Vector2Df position, float speed, float decay, float radius, float hitbox_off, float angle = 0.f) // temporary for testing
	{
		lecs::Entity* projectile = &game.load()->ecs_managers.entity_manager->AddEntity();
		SpriteComponent* sprite = projectile->AddComponent<SpriteComponent>("bullet");
		sprite->sprite.setRotation(angle);
		sprite->SetDrawOrderPoint(64.f);
		TransformComponent* transform = projectile->AddComponent<TransformComponent>(position, sprite->sprite.getTextureRect().width,
			sprite->sprite.getTextureRect().height, game.load()->world_scale);
		transform->speed = speed;
		if (angle > 90 && angle < 270 || angle < -90 && angle > -270) transform->scale.y = fabsf(transform->scale.y) * -1;
		ProjectileComponent* projectileC = projectile->AddComponent<ProjectileComponent>(position, angle, decay);
		ProjHitBoxComponent* phb = projectile->AddComponent<ProjHitBoxComponent>(radius, hitbox_off, angle);

		game.load()->ecs_managers.entity_manager->AddToGroup(projectile, lecs::GRP_ENTITY);

		return projectile;
	}

	inline lecs::Entity* Player(Vector2Df position)
	{
		lecs::Entity* player = &game.load()->ecs_managers.entity_manager->AddEntity();
		ItemComponent* item = player->AddComponent<ItemComponent>(Weapon(Vector2Df::Zero(), false), 32.f, true);
		SpriteComponent* sprite = player->AddComponent<SpriteComponent>("player", Vector2Df(16.f, 16.f));
		TransformComponent* transform = player->AddComponent<TransformComponent>(position, 32, 32, game.load()->world_scale, 4.f);
		ColliderComponent* collider = player->AddComponent<ColliderComponent>(4 * game.load()->world_scale.x, 12 * game.load()->world_scale.y, true, true, ColliderComponent::TAG::PLAYER);
		collider->SetOffset(Vector2Df(0.f, 3.f) * game.load()->world_scale + Vector2Df(0.f, transform->height));
		AnimationComponent* animation = player->AddComponent<AnimationComponent>();
		animation->AddAnimation("idle", 0, 2, 300);
		animation->AddAnimation("walk", 1, 8, 100);
		BodyHitBoxComponent* bdhb = player->AddComponent<BodyHitBoxComponent>(Vector2Df(0.f, 18.f), 20);
		HeadHitBoxComponent* hhb = player->AddComponent<HeadHitBoxComponent>(Vector2Df(0.f, -10.f), 16);
		HealthComponent* hp = player->AddComponent<HealthComponent>(99999.f);
		hp->SetHealthBar({ 0.f, -32.f });

		game.load()->ecs_managers.entity_manager->AddToGroup(player, lecs::GRP_ENTITY);
		game.load()->ecs_managers.entity_manager->AddToGroup(player, lecs::GRP_PLAYER);

		return player;
	}

	inline lecs::Entity* Enemy(Vector2Df position, int room)
	{
		lecs::Entity* enemy = &game.load()->ecs_managers.entity_manager->AddEntity();
		ItemComponent* item = enemy->AddComponent<ItemComponent>(Weapon(Vector2Df::Zero(), false), 32.f, true);
		SpriteComponent* sprite = enemy->AddComponent<SpriteComponent>("enemy", Vector2Df(16.f, 16.f));
		TransformComponent* transform = enemy->AddComponent<TransformComponent>(position, 32, 32, game.load()->world_scale, 4.f);
		ColliderComponent* collider = enemy->AddComponent<ColliderComponent>(4 * game.load()->world_scale.x, 12 * game.load()->world_scale.y, true, true, ColliderComponent::TAG::ENEMY);
		collider->SetOffset(Vector2Df(0.f, 3.f) * game.load()->world_scale + Vector2Df(0.f, transform->height));
		AnimationComponent* animation = enemy->AddComponent<AnimationComponent>();
		animation->AddAnimation("idle", 0, 2, 300);
		animation->AddAnimation("walk", 1, 8, 100);
		animation->AddAnimation("dead", 2, 1, 0);
		BodyHitBoxComponent* bdhb = enemy->AddComponent<BodyHitBoxComponent>(Vector2Df(0.f, 18.f), 20);
		HeadHitBoxComponent* hhb = enemy->AddComponent<HeadHitBoxComponent>(Vector2Df(0.f, -10.f), 16);
		HealthComponent* hp = enemy->AddComponent<HealthComponent>(100.f);
		hp->SetHealthBar({ 0.f, -32.f });
		AIComponent* ai = enemy->AddComponent<AIComponent>(room);

		game.load()->ecs_managers.entity_manager->AddToGroup(enemy, lecs::GRP_ENTITY);
		game.load()->ecs_managers.entity_manager->AddToGroup(enemy, lecs::GRP_ENEMY);

		game.load()->ai_manager->AddEnemy(ai);

		return enemy;
	}
}