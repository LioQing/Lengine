#pragma once

#include <lecs.h>

struct HitBoxSystem : public lecs::System
{
	bool draw_characters = false;
	bool draw_proj = false;

	void EarlyUpdate(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, DeltaTime dt);
	void Draw(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, sf::RenderWindow* window) override;
};