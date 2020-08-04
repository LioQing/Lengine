#include "ItemSystem.h"

#include "../SpawnEntity.h"
#include "../Game.h"
#include "../Components/Components.h"

extern std::atomic<Game*> game;

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
				game.load()->logger->AddLog
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

		if (e->HasComponent<HealthComponent>() && e->GetComponent<HealthComponent>().is_dead) 
			continue;

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

			if (gun->ready)
			{
				Vector2Df position;

				// fire function
				auto Fire = [&]()
				{
					float rad = atan2f(position.y, position.x);

					float tmp_y_pos = gun->muzzle_pos.y;
					if (position.x < 0) tmp_y_pos *= -1;

					Vector2Df g_muzzle_pos =
						item->center +
						Vector2Df((item->radius + gun->muzzle_pos.x) * cosf(rad), (item->radius + gun->muzzle_pos.x) * sinf(rad)) +
						Vector2Df(tmp_y_pos * sinf(rad), -tmp_y_pos * cosf(rad));

					lecs::Entity* proj = spawn::Projectile(g_muzzle_pos, 4.f, 2400.f, 5.f, 10.f, rad * 180 / M_PI); // to be changed
					gun->ready = false;
				};

				// player
				if (e->HasGroup(lecs::GRP_PLAYER) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					position = game.load()->input_manager.world_mouse_pos - item->center;
					Fire();
				}
				// bot
				else if (e->HasGroup(lecs::GRP_ENEMY) && e->GetComponent<AIComponent>().is_firing.load() &&
					!entity_manager->GetGroup(lecs::GRP_PLAYER).entities.at(0)->GetComponent<HealthComponent>().is_dead)
				{
					position = *e->GetComponent<AIComponent>().gun_pt_dir.load();
					Fire();
				}
			}
		}
	}
}

void ItemSystem::Update(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, DeltaTime delta_time)
{
	for (auto& e : entity_manager->EntityFilter<ItemComponent>().entities)
	{
		ItemComponent* item = &e->GetComponent<ItemComponent>();

		if (e->HasGroup(lecs::GRP_PLAYER)) item->RenderSprite(game.load()->input_manager.world_mouse_pos - item->center);
		else if (e->HasGroup(lecs::GRP_ENEMY)) item->RenderSprite(*e->GetComponent<AIComponent>().gun_pt_dir.load());
	}

	// pick up items
	static bool isEDown = false;
	auto& player_trans = game.load()->player->GetComponent<TransformComponent>();

	for (auto& e : entity_manager->EntityFilter<BaseItemComponent>().entities)
	{
		BaseItemComponent* bitem = &e->GetComponent<BaseItemComponent>();
		
		if (bitem->isDropped)
		{
			if (e->GetComponent<TransformComponent>().position.Distance(player_trans.position + Vector2Df(0.f, 12.f) * game.load()->world_scale) > 16.f * game.load()->world_scale.x) // pick up range
				continue;

			if (!isEDown && sf::Keyboard::isKeyPressed(sf::Keyboard::E))
			{
				auto& eitem = game.load()->player->GetComponent<ItemComponent>().item;
				eitem->GetComponent<BaseItemComponent>().isDropped = true;
				eitem->GetComponent<SpriteComponent>().SetDrawOrderPoint(0.f);
				eitem->GetComponent<TransformComponent>().position = player_trans.position + Vector2Df(0.f, 12.f) * game.load()->world_scale;

				bitem->isDropped = false;
				game.load()->player->GetComponent<ItemComponent>().item = e;
				e->GetComponent<SpriteComponent>().SetDrawOrderPoint(BaseItemComponent::equipped_dop);

				break;
			}
		}
	}

	isEDown = sf::Keyboard::isKeyPressed(sf::Keyboard::E);
}
