#pragma once

#include <lecs.h>

struct ProjectileSystem : public lecs::System
{
	void EarlyUpdate(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, DeltaTime delta_time);
};