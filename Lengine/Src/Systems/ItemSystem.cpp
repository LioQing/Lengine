#include "ItemSystem.h"

#include "../Game.h"
#include "../Components/Components.h"

extern Game* game;

void ItemSystem::HandleInput(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, DeltaTime delta_time)
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
				if (item->scale_transform) item->sprite.setScale(transform->scale.sfVector2f());
			}
		}
	}
}

void ItemSystem::Draw(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, sf::RenderWindow* window)
{
	for (auto& e : entity_manager->EntityFilter<ItemComponent>().entities)
	{
		ItemComponent* item = &e->GetComponent<ItemComponent>();

		item->UpdateSprite();

		window->draw(item->sprite);
	}
}
