#pragma once

#include <lecs.h>

#include "Components/Components.h"
#include "Game.h"

extern Game* game;

// to do: add data file to directly load properties of components of entities

namespace spawn
{
	inline lecs::Entity* StaticObject(Vector2Df position, int tile_size, std::string texture_id)
	{
		lecs::Entity* static_obj = &game->ecs_managers.entity_manager->AddEntity();
		SpriteComponent* sprite = &static_obj->AddComponent<SpriteComponent>(
			texture_id,
			Vector2Df(0.f, game->texture_manager.GetTexture(texture_id).getSize().y - tile_size)
			);
		TransformComponent* transform = &static_obj->AddComponent<TransformComponent>(
			position,
			sprite->sprite.getTextureRect().width,
			sprite->sprite.getTextureRect().height,
			game->world_scale);
		ColliderComponent* collider = &static_obj->AddComponent<ColliderComponent>(
			position + Vector2Df(16.f, 16.f) * game->world_scale,
			32 * game->world_scale.x, 32 * game->world_scale.y,
			false
			);

		game->ecs_managers.entity_manager->AddToGroup(static_obj, lecs::GRP_ENTITY);

		return static_obj;
	}

	inline lecs::Entity* Staircase(Vector2Df position, std::string texture_id)
	{
		lecs::Entity* staircase = &game->ecs_managers.entity_manager->AddEntity();
		SpriteComponent* sprite = &staircase->AddComponent<SpriteComponent>(
			texture_id,
			Vector2Df(0.f, 0.f)
			);
		TransformComponent* transform = &staircase->AddComponent<TransformComponent>(
			position,
			sprite->sprite.getTextureRect().width,
			sprite->sprite.getTextureRect().height,
			game->world_scale);

		game->ecs_managers.entity_manager->AddToGroup(staircase, lecs::GRP_BACKGROUND_ENTITY);

		return staircase;
	}

	inline lecs::Entity* Map()
	{
		int map_size = 50;
		int floor_i = 0;
		int wall_i = 2;
		int side_wall_i = 1;
		int tile_size = 32;

		lecs::Entity* map = &game->ecs_managers.entity_manager->AddEntity();
		TileMapComponent* tilemap = &map->AddComponent<TileMapComponent>("terrain", tile_size, game->world_scale.x);
		LevelComponent* level = &map->AddComponent<LevelComponent>(-1);
		level->GenMap(8, map_size, 5, 10, floor_i, wall_i, side_wall_i);
		tilemap->top_layer_i = 2;
		tilemap->LoadMap(level->map);
		level->SetStaticTextures({ "crate", "d_crate" });
		level->GenStatics(5, 8, 32, game->ecs_managers.entity_manager);
		level->GenStair(32);
		BoundaryComponent* boundary = &map->AddComponent<BoundaryComponent>(level->map, wall_i, side_wall_i, map_size, map_size, tile_size * game->world_scale.x, Vector2Df(0.f ,0.f) * game->world_scale);

		game->ecs_managers.entity_manager->AddToGroup(map, lecs::GRP_TERRAIN);

		return map;
	}

	inline lecs::Entity* Weapon()
	{
		lecs::Entity* item = &game->ecs_managers.entity_manager->AddEntity();
		SpriteComponent* sprite = &item->AddComponent<SpriteComponent>("glock");
		TransformComponent* transform = &item->AddComponent<TransformComponent>(Vector2Df(400, 320), sprite->sprite.getTextureRect().width,
			sprite->sprite.getTextureRect().height, game->world_scale);
		GunComponent* gun = &item->AddComponent<GunComponent>(
			Vector2Df
			(
				sprite->sprite.getTextureRect().width * game->world_scale.x, 
				sprite->sprite.getTextureRect().height * game->world_scale.y / 2 - 3
			),
			65.f);

		game->ecs_managers.entity_manager->AddToGroup(item, lecs::GRP_ENTITY);

		return item;
	}

	inline lecs::Entity* Projectile(Vector2Df position, float speed, float decay, float angle = 0.f) // temporary for testing
	{
		lecs::Entity* projectile = &game->ecs_managers.entity_manager->AddEntity();
		SpriteComponent* sprite = &projectile->AddComponent<SpriteComponent>("bullet");
		sprite->sprite.setRotation(angle);
		TransformComponent* transform = &projectile->AddComponent<TransformComponent>(position, sprite->sprite.getTextureRect().width,
			sprite->sprite.getTextureRect().height, game->world_scale);
		transform->speed = speed;
		if (angle > 90 && angle < 270 || angle < -90 && angle > -270) transform->scale.y = fabsf(transform->scale.y) * -1;
		ProjectileComponent* projectileC = &projectile->AddComponent<ProjectileComponent>(angle, decay);

		game->ecs_managers.entity_manager->AddToGroup(projectile, lecs::GRP_ENTITY);

		return projectile;
	}

	inline lecs::Entity* Player(Vector2Df position)
	{
		lecs::Entity* player = &game->ecs_managers.entity_manager->AddEntity();
		ItemComponent* item = &player->AddComponent<ItemComponent>(Weapon(), 32.f, true);
		SpriteComponent* sprite = &player->AddComponent<SpriteComponent>("player", Vector2Df(16.f, 16.f));
		TransformComponent* transform = &player->AddComponent<TransformComponent>(position, 32, 32, Vector2Df(3.f, 3.f), 4.f);
		ColliderComponent* collider = &player->AddComponent<ColliderComponent>(4 * game->world_scale.x, 16 * game->world_scale.y, true, true);
		collider->SetOffset(Vector2Df(0.f, 3.f) * game->world_scale + Vector2Df(0.f, transform->height));
		AnimationComponent* animation = &player->AddComponent<AnimationComponent>();
		animation->AddAnimation("idle", 0, 2, 300);
		animation->AddAnimation("walk", 1, 8, 100);

		game->ecs_managers.entity_manager->AddToGroup(player, lecs::GRP_ENTITY);
		game->ecs_managers.entity_manager->AddToGroup(player, lecs::GRP_PLAYER);

		return player;
	}
}