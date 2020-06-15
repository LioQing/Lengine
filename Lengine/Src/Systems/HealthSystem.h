#pragma once

#include <lecs.h>

struct HealthSystem : public lecs::System, public lecs::EventSubscriber
{
	void Init(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, lecs::SystemManager* system_manager) override;
	void Receive(lecs::Event& event) override;
};