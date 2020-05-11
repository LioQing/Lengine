#pragma once

#include <lecs.h>

struct TileMapSystem : public lecs::System
{
	void Draw(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, sf::RenderWindow* window) override;
	void LateDraw(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, sf::RenderWindow* window);
};