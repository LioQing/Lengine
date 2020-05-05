#pragma once

#include <lecs.h>

struct ItemSystem : public lecs::System 
{
	void EarlyUpdate(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, DeltaTime delta_time);
	void LateUpdate(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, DeltaTime delta_time);
	void Update(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, DeltaTime delta_time) override;
};