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
			if (
				s.getGlobalBounds().left > game->camera_manager.GetViewBounds().left + game->camera_manager.GetViewBounds().width ||
				s.getGlobalBounds().top > game->camera_manager.GetViewBounds().top + game->camera_manager.GetViewBounds().height ||
				s.getGlobalBounds().left + s.getGlobalBounds().width < game->camera_manager.GetViewBounds().left ||
				s.getGlobalBounds().top + s.getGlobalBounds().height < game->camera_manager.GetViewBounds().top
				)
			{
				continue;
			}

			window->draw(s);
		}
	}
}
