#pragma once

#include <lecs.h>

#include "../Components/Components.h"

struct ProjectileSystem : public lecs::System
{
	void EarlyUpdate(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, DeltaTime delta_time);
	void Update(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, DeltaTime delta_time) override;

	bool HitWallDetect(ColliderComponent* c, lecs::Entity* e);
};