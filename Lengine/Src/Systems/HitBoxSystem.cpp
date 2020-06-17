#include "HitBoxSystem.h"

#include <SFML/Graphics.hpp>

#include "../Game.h"
#include "../Components/Components.h"
#include "../Events/Events.h"

extern Game* game;

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

			phb->circle.setPosition(transform->position.sfVector2f());

			if (!game->InsideView(phb->circle.getGlobalBounds())) continue;
			window->draw(phb->circle);
		}
	}

	if (!draw_characters) return;

	for (auto& character : entity_manager->EntityFilter<BodyHitBoxComponent>().entities)
	{
		auto* bdhb = &character->GetComponent<BodyHitBoxComponent>();
		auto* transform = &character->GetComponent<TransformComponent>();

		bdhb->circle.setPosition((transform->position + bdhb->position).sfVector2f());
		
		if (!game->InsideView(bdhb->circle.getGlobalBounds())) continue;
		window->draw(bdhb->circle);

		if (!character->HasComponent<HeadHitBoxComponent>()) continue;

		auto* hhb = &character->GetComponent<HeadHitBoxComponent>();

		hhb->circle.setPosition((transform->position + hhb->position).sfVector2f());

		if (!game->InsideView(hhb->circle.getGlobalBounds())) continue;
		window->draw(hhb->circle);
	}
}
