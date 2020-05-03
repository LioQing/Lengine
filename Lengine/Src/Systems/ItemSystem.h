#pragma once

#include <lecs.h>

struct ItemSystem : public lecs::System 
{
	void HandleInput(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, DeltaTime delta_time) override;
	void Draw(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, sf::RenderWindow* window) override;
};