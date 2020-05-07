#include "ItemSystem.h"

#include "../SpawnEntity.h"
#include "../Game.h"
#include "../Components/Components.h"

extern Game* game;

void ItemSystem::EarlyUpdate(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, DeltaTime delta_time)
{
	for (auto& e : entity_manager->EntityFilter<ItemComponent>().entities)
	{
		ItemComponent* item = &e->GetComponent<ItemComponent>();

		// follow transform
		if (item->follow_transform || item->scale_transform)
		{
			if (!e->HasComponent<TransformComponent>())
			{
				game->logger->AddLog
				(
					"Error: Entity " + std::to_string(e->id) + " doesn't have Transform Component for item",
					lecs::LT_ERROR, lecs::LT_ENTITY, lecs::LT_COMPONENT
				);
			}
			else
			{
				TransformComponent* transform = &e->GetComponent<TransformComponent>();

				if (item->follow_transform) item->center = transform->position;
			}
		}
	}
}

void ItemSystem::LateUpdate(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, DeltaTime delta_time)
{
	for (auto& e : entity_manager->EntityFilter<ItemComponent>().entities)
	{
		ItemComponent* item = &e->GetComponent<ItemComponent>();

		if (item->item->HasComponent<GunComponent>())
		{
			SpriteComponent* sprite = &item->item->GetComponent<SpriteComponent>();
			GunComponent* gun = &item->item->GetComponent<GunComponent>();

			if (!gun->ready)
			{
				gun->fire_rate_timer += delta_time;
				if (gun->fire_rate_timer > gun->fire_rate)
				{
					gun->fire_rate_timer -= gun->fire_rate;
					gun->ready = true;
				}
			}

			if (gun->ready && sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				Vector2Df position = game->input_manager.world_mouse_pos - item->center;
				float rad = atan2f(position.y, position.x);

				float tmp_y_pos = gun->muzzle_pos.y;
				if (position.x < 0) tmp_y_pos *= -1;

				Vector2Df g_muzzle_pos = 
					item->center + 
					Vector2Df((item->radius + gun->muzzle_pos.x) * cosf(rad), (item->radius + gun->muzzle_pos.x) * sinf(rad))
					+ Vector2Df(tmp_y_pos * sinf(rad), -tmp_y_pos * cosf(rad));

				lecs::Entity* proj = spawn::Projectile(g_muzzle_pos, 15.f, 2400.f, rad * 180 / M_PI);
				gun->ready = false;
			}
		}
	}
}

void ItemSystem::Update(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, DeltaTime delta_time)
{
	for (auto& e : entity_manager->EntityFilter<ItemComponent>().entities)
	{
		ItemComponent* item = &e->GetComponent<ItemComponent>();

		if (e->HasGroup(lecs::GRP_PLAYER)) item->RenderSprite(game->input_manager.world_mouse_pos - item->center);
		else item->RenderSprite();
	}
}
