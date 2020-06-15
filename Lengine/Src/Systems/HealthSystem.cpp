#include "HealthSystem.h"

#include "../Events/Events.h"

void HealthSystem::Init(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, lecs::SystemManager* system_manager)
{
	event_manager->Subscribe<HitBoxEvent>(this);
}

void HealthSystem::Receive(lecs::Event& event)
{
	if (event.IsEvent<HitBoxEvent>())
	{
		HitBoxEvent hbev = event.Downcast<HitBoxEvent>();
		std::cout << hbev.projectile->id << " hit " << hbev.hitbox->id;
		if (hbev.ishead) std::cout << " head";
		std::cout << std::endl;
	}
}
