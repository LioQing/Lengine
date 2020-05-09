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
		int map_size = 50;
		int floor_i = 0;
		int wall_i = 2;
		int side_wall_i = 1;
		int tile_size = 32;

		lecs::Entity* map = &game->ecs_managers.entity_manager->AddEntity();
		TileMapComponent* tilemap = &map->AddComponent<TileMapComponent>("terrain", tile_size, game->world_scale.x);
		LevelComponent* level = &map->AddComponent<LevelComponent>(-1);
		level->GenMap(8, map_size, 5, 10, floor_i, wall_i, side_wall_i);
		tilemap->LoadMap(level->map);
		BoundaryComponent* boundary = &map->AddComponent<BoundaryComponent>(level->map, wall_i, side_wall_i, map_size, map_size, tile_size * game->world_scale.x);

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

	inline lecs::Entity* Player(Vector2Df position)
	{
		lecs::Entity* player = &game->ecs_managers.entity_manager->AddEntity();
		ItemComponent* item = &player->AddComponent<ItemComponent>(Weapon(), 32.f, true);
		SpriteComponent* sprite = &player->AddComponent<SpriteComponent>("player", Vector2Df(16.f, 16.f));
		TransformComponent* transform = &player->AddComponent<TransformComponent>(position, 32, 32, Vector2Df(3.f, 3.f), 4.f);
		ColliderComponent* collider = &player->AddComponent<ColliderComponent>(96, 48, true, true);
		AnimationComponent* animation = &player->AddComponent<AnimationComponent>();
		animation->AddAnimation("idle", 0, 2, 300);
		animation->AddAnimation("walk", 1, 8, 100);
		game->ecs_managers.entity_manager->AddToGroup(player, lecs::GRP_PLAYER);

		return player;
	}
}