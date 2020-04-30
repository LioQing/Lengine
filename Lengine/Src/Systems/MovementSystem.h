#pragma once

#include <lecs.h>

#include "../Components/Components.h"

struct MovementSystem : public lecs::System
{
	void Update(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, DeltaTime dt) override
	{
		for (auto& e : entity_manager->EntityFilter<TransformComponent>().EntityFilter<SpriteComponent>().entities)
		{
			TransformComponent* transform = &e->GetComponent<TransformComponent>();
			SpriteComponent* sprite = &e->GetComponent<SpriteComponent>();

			sprite->sprite.setPosition(transform->position);
		}
	}
};