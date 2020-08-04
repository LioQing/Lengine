#include "AIMovementSystem.h"

#include <SFML/System.hpp>

#include "../../Components/Components.h"

void AIMovementSystem::HandleInput(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, DeltaTime delta_time)
{
	for (auto& e : entity_manager->GetGroup(lecs::GRP_ENEMY).EntityFilter<TransformComponent>().EntityFilter<AIComponent>().entities)
	{
		TransformComponent* transform = &e->GetComponent<TransformComponent>();
		AIComponent* ai = &e->GetComponent<AIComponent>();

		// ensure no x speed
		if (transform->x_speed != 0)
			transform->x_speed = 0;

		// movement
		transform->velocity = ai->movement.load()->Cast<float>();

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