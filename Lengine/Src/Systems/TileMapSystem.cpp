#include "TileMapSystem.h"

#include "../Components/Components.h"

void TileMapSystem::Render(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, sf::RenderWindow* window)
{
	for (auto& e : entity_manager->EntityFilter<TileMapComponent>().entities)
	{
		window->draw(e->GetComponent<TileMapComponent>());
	}
}
