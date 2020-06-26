#pragma once

#include <lecs.h>

struct AIMovementSystem : public lecs::System
{
	void HandleInput(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, DeltaTime delta_time) override;
};