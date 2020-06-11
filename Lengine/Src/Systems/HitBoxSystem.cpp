#include "HitBoxSystem.h"

#include <SFML/Graphics.hpp>

#include "../Components/Components.h"

void HitBoxSystem::EarlyUpdate(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, DeltaTime dt)
{
	
}

void HitBoxSystem::Draw(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, sf::RenderWindow* window)
{
	for (auto& proj : entity_manager->EntityFilter<ProjHitBoxComponent>().entities)
	{
		auto* phb = &proj->GetComponent<ProjHitBoxComponent>();
		auto* transform = &proj->GetComponent<TransformComponent>();

		sf::CircleShape circle;

		circle.setRadius(phb->radius);
		circle.setFillColor(sf::Color(sf::Color::Transparent));
		circle.setOutlineColor(sf::Color(sf::Color::Magenta));
		circle.setOrigin(circle.getRadius(), circle.getRadius());
		circle.setPosition(transform->position.sfVector2f());
		circle.setOutlineThickness(3.f);

		window->draw(circle);
	}
}
