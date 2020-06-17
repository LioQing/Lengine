#include "TileMapSystem.h"

#include "../Components/Components.h"
#include "../Game.h"

extern Game* game;

void TileMapSystem::Draw(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, sf::RenderWindow* window)
{
	for (auto& e : entity_manager->EntityFilter<TileMapComponent>().entities)
	{
		TileMapComponent* tilemap = &e->GetComponent<TileMapComponent>();

		// draw
		for (auto& s : tilemap->tiles)
		{
			// off screen no draw
			if (!game->InsideView(s.getGlobalBounds())) continue;

			window->draw(s);
		}
	}
}

void TileMapSystem::LateDraw(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, sf::RenderWindow* window)
{
	for (auto& e : entity_manager->EntityFilter<TileMapComponent>().entities)
	{
		TileMapComponent* tilemap = &e->GetComponent<TileMapComponent>();

		// draw
		for (auto& s : tilemap->top_layer_tiles)
		{
			// off screen no draw
			if (!game->InsideView(s.getGlobalBounds())) continue;

			window->draw(s);
		}
	}
}
