#include "MovementSystem.h"

#include <SFML/System.hpp>

#include "../Components/Components.h"

void MovementSystem::HandleInput(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, DeltaTime delta_time)
{
	for (auto& e : entity_manager->GetGroup(lecs::GRP_PLAYER).EntityFilter<TransformComponent>().EntityFilter<SpriteComponent>().entities)
	{
		TransformComponent* transform = &e->GetComponent<TransformComponent>();
		SpriteComponent* sprite = &e->GetComponent<SpriteComponent>();

		// y axis movement
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			transform->velocity.y = -1;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && !sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			transform->velocity.y = 1;
		}
		else
		{
			transform->velocity.y = 0;
		}

		// x axis movement
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && !sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			transform->velocity.x = -1;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && !sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			transform->velocity.x = 1;
		}
		else
		{
			transform->velocity.x = 0;
		}

		// diagonal movement
		if (transform->velocity.Magnitude() > 1)
		{
			transform->velocity = transform->velocity.Normalize();
		}

		// animation
		if (e->HasComponent<AnimationComponent>())
		{
			AnimationComponent* animation = &e->GetComponent<AnimationComponent>();
			
			if (transform->velocity.Magnitude() > 0)
			{
				animation->SetCurrent("walk");
			}
			else
			{
				animation->SetCurrent("idle");
			}
		}

		// sprite flip
		if (transform->velocity.x < 0)
		{
			transform->scale.x = fabsf(transform->scale.x) * -1;
		}
		else if (transform->velocity.x > 0)
		{
			transform->scale.x = fabsf(transform->scale.x);
		}
	}
}

void MovementSystem::EarlyUpdate(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, DeltaTime delta_time)
{
	for (auto& e : entity_manager->EntityFilter<TransformComponent>().entities)
	{
		TransformComponent* transform = &e->GetComponent<TransformComponent>();

		transform->position += transform->velocity * transform->speed * delta_time * 6 / 100;
	}
}

void MovementSystem::LateUpdate(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, DeltaTime delta_time)
{
	for (auto& e : entity_manager->EntityFilter<TransformComponent>().EntityFilter<SpriteComponent>().entities)
	{
		TransformComponent* transform = &e->GetComponent<TransformComponent>();
		SpriteComponent* sprite = &e->GetComponent<SpriteComponent>();

		sprite->sprite.setPosition(transform->position.sfVector2f());
	}
}