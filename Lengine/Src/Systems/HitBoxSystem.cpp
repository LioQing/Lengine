#include "HitBoxSystem.h"

#include <SFML/Graphics.hpp>

#include "../Components/Components.h"
#include "../Events/Events.h"

void HitBoxSystem::EarlyUpdate(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, DeltaTime dt)
{
	for (auto& proj : entity_manager->EntityFilter<ProjHitBoxComponent>().entities)
	{
		auto* phb = &proj->GetComponent<ProjHitBoxComponent>();
		auto* transform = &proj->GetComponent<TransformComponent>();

		for (auto& body : entity_manager->EntityFilter<BodyHitBoxComponent>().entities)
		{
			TransformComponent* hTransform = &body->GetComponent<TransformComponent>();

			if (body->HasComponent<HeadHitBoxComponent>())
			{
				HeadHitBoxComponent* hhb = &body->GetComponent<HeadHitBoxComponent>();

				if ((hTransform->position + hhb->position).Distance(transform->position) <= hhb->radius + phb->radius)
				{
					proj->Destroy();
					event_manager->Emit<HitBoxEvent>(proj, body, true);
				}
			}

			BodyHitBoxComponent* bhb = &body->GetComponent<BodyHitBoxComponent>();

			if ((hTransform->position + bhb->position).Distance(transform->position) <= bhb->radius + phb->radius)
			{
				proj->Destroy();
				event_manager->Emit<HitBoxEvent>(proj, body, false);
			}
		}
	}
}

void HitBoxSystem::Draw(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, sf::RenderWindow* window)
{
	if (draw_proj)
	{
		for (auto& proj : entity_manager->EntityFilter<ProjHitBoxComponent>().entities)
		{
			auto* phb = &proj->GetComponent<ProjHitBoxComponent>();
			auto* transform = &proj->GetComponent<TransformComponent>();

			sf::CircleShape circle;

			circle.setRadius(phb->radius);
			circle.setFillColor(sf::Color(sf::Color::Transparent));
			circle.setOutlineColor(sf::Color(sf::Color::Green));
			circle.setOrigin(circle.getRadius(), circle.getRadius());
			circle.setPosition(transform->position.sfVector2f());
			circle.setOutlineThickness(3.f);

			window->draw(circle);
		}
	}

	if (!draw_characters) return;

	for (auto& character : entity_manager->EntityFilter<BodyHitBoxComponent>().entities)
	{
		auto* bdhb = &character->GetComponent<BodyHitBoxComponent>();
		auto* transform = &character->GetComponent<TransformComponent>();

		sf::CircleShape bdcircle;

		bdcircle.setRadius(bdhb->radius);
		bdcircle.setFillColor(sf::Color(sf::Color::Transparent));
		bdcircle.setOutlineColor(sf::Color(sf::Color::Green));
		bdcircle.setOrigin(bdcircle.getRadius(), bdcircle.getRadius());
		bdcircle.setPosition((transform->position + bdhb->position).sfVector2f());
		bdcircle.setOutlineThickness(3.f);

		window->draw(bdcircle);

		if (!character->HasComponent<HeadHitBoxComponent>()) continue;

		auto* hhb = &character->GetComponent<HeadHitBoxComponent>();

		bdcircle.setRadius(hhb->radius);
		bdcircle.setFillColor(sf::Color(sf::Color::Transparent));
		bdcircle.setOutlineColor(sf::Color(sf::Color::Green));
		bdcircle.setOrigin(bdcircle.getRadius(), bdcircle.getRadius());
		bdcircle.setPosition((transform->position + hhb->position).sfVector2f());
		bdcircle.setOutlineThickness(3.f);

		window->draw(bdcircle);
	}
}
