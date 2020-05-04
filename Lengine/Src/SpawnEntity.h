#pragma once

#include <lecs.h>

#include "Components/Components.h"
#include "Game.h"

extern Game* game;

namespace spawn
{
	lecs::Entity* Map()
	{
		lecs::Entity* map = &game->ecs_managers.entity_manager->AddEntity();
		BoundaryComponent* boundary = &map->AddComponent<BoundaryComponent>("Assets/Boundary.csv", 20, 20, 96);
		TileMapComponent* tilemap = &map->AddComponent<TileMapComponent>("terrain", 32, 3);
		tilemap->LoadMap("Assets/Island.csv", 20, 20);

		return map;
	}

	lecs::Entity* Weapon()
	{
		lecs::Entity* item = &game->ecs_managers.entity_manager->AddEntity();
		SpriteComponent* sprite = &item->AddComponent<SpriteComponent>("glock");
		TransformComponent* transform = &item->AddComponent<TransformComponent>(Vector2Df(400, 320), sprite->sprite.getTextureRect().width,
			sprite->sprite.getTextureRect().height, Vector2Df(3.f, 3.f));

		return item;
	}

	lecs::Entity* Player()
	{
		lecs::Entity* player = &game->ecs_managers.entity_manager->AddEntity();
		ItemComponent* item = &player->AddComponent<ItemComponent>(Weapon(), 32.f, true);
		SpriteComponent* sprite = &player->AddComponent<SpriteComponent>("player", Vector2Df(16.f, 16.f));
		TransformComponent* transform = &player->AddComponent<TransformComponent>(Vector2Df(400, 320), 32, 32, Vector2Df(3.f, 3.f), 2.4f);
		ColliderComponent* collider = &player->AddComponent<ColliderComponent>(96, 48, true, true);
		AnimationComponent* animation = &player->AddComponent<AnimationComponent>();
		animation->AddAnimation("idle", 0, 2, 300);
		animation->AddAnimation("walk", 1, 8, 100);
		game->ecs_managers.entity_manager->AddToGroup(player, lecs::GRP_PLAYER);

		return player;
	}
}