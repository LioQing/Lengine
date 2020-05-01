#pragma once

#include <lecs.h>

class TileMapSystem : public lecs::System
{
public:

	void Render(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, sf::RenderWindow* window) override;
};