#pragma once

#include <lecs.h>

#include "Components/Components.h"
#include "Game.h"

extern Game* game;

// to do: add data file to directly load properties of components of entities

namespace spawn
{
	inline lecs::Entity* Map()
	{
		lecs::Entity* map = &game->ecs_managers.entity_manager->AddEntity();
		//BoundaryComponent* boundary = &map->AddComponent<BoundaryComponent>("Assets/Boundary.csv", 20, 20, 96);
		TileMapComponent* tilemap = &map->AddComponent<TileMapComponent>("terrain", 32, game->world_scale.x);
		LevelComponent* level = &map->AddComponent<LevelComponent>(-1);
		level->GenMap(8, 50, 5, 10, 4, 13, 1);
		tilemap->LoadMap(level->map);

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

		return projectile;
	}

	inline lecs::Entity* Player()
	{
		lecs::Entity* player = &game->ecs_managers.entity_manager->AddEntity();
		ItemComponent* item = &player->AddComponent<ItemComponent>(Weapon(), 32.f, true);
		SpriteComponent* sprite = &player->AddComponent<SpriteComponent>("player", Vector2Df(16.f, 16.f));
		TransformComponent* transform = &player->AddComponent<TransformComponent>(Vector2Df(400, 320), 32, 32, Vector2Df(3.f, 3.f), 4.f);
		ColliderComponent* collider = &player->AddComponent<ColliderComponent>(96, 48, true, true);
		AnimationComponent* animation = &player->AddComponent<AnimationComponent>();
		animation->AddAnimation("idle", 0, 2, 300);
		animation->AddAnimation("walk", 1, 8, 100);
		game->ecs_managers.entity_manager->AddToGroup(player, lecs::GRP_PLAYER);

		return player;
	}
}