#pragma once

#include <lecs.h>

struct MovementSystem : public lecs::System
{
	void HandleInput(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, DeltaTime delta_time) override;
	void LateUpdate(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, DeltaTime delta_time);
};