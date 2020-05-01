#include "TileMapSystem.h"

#include "../Components/Components.h"

void TileMapSystem::Draw(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, sf::RenderWindow* window)
{
	for (auto& e : entity_manager->EntityFilter<TileMapComponent>().entities)
	{
		TileMapComponent* tilemap = &e->GetComponent<TileMapComponent>();

		// draw
		for (auto& s : tilemap->tiles)
		{
			// off screen no draw
			if (
				s.getGlobalBounds().left > window->getSize().x ||
				s.getGlobalBounds().top > window->getSize().y ||
				s.getGlobalBounds().left + s.getGlobalBounds().width < 0 ||
				s.getGlobalBounds().top + s.getGlobalBounds().height < 0
				)
			{
				continue;
			}

			window->draw(s);
		}
	}
}
