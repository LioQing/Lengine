#include "CollisionSystem.h"

#include "../Game.h"

extern Game* game;

void CollisionSystem::EarlyUpdate(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, DeltaTime dt)
{
	for (auto& e : entity_manager->EntityFilter<ColliderComponent>().entities)
	{
		ColliderComponent* col = &e->GetComponent<ColliderComponent>();
		
		if (col->fitTransform || col->followTransform)
		{
			if (!e->HasComponent<TransformComponent>())
			{
				game->logger->AddLog
				(
					"Error: Entity " + std::to_string(e->id) + " doesn't have Transform Component for collider",
					lecs::LT_ERROR, lecs::LT_ENTITY, lecs::LT_COMPONENT
				);
				continue;
			}

			TransformComponent* transform = &e->GetComponent<TransformComponent>();

			if (col->fitTransform)
			{
				col->height = transform->height * transform->scale.y;
				col->width = transform->width * transform->scale.x;
			}
			if (col->followTransform)
			{
				col->position = transform->position;
			}
		}
	}
}

void CollisionSystem::Draw(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, sf::RenderWindow* window)
{
	for (auto& e : entity_manager->EntityFilter<ColliderComponent>().entities)
	{
		ColliderComponent* col = &e->GetComponent<ColliderComponent>();
		col->UpdateBox();
		window->draw(col->box);
	}

	for (auto& e2 : entity_manager->EntityFilter<BoundaryComponent>().entities)
	{
		for (auto boundary_col : e2->GetComponent<BoundaryComponent>().boundaries)
		{
			if (boundary_col == nullptr) continue;
			boundary_col->UpdateBox();
			window->draw(boundary_col->box);
		}
	}
}

void CollisionSystem::Update(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, DeltaTime dt)
{
	for (auto& e1 : entity_manager->EntityFilter<ColliderComponent>().entities)
	{
		ColliderComponent* kinematic_col = &e1->GetComponent<ColliderComponent>();
		if (!kinematic_col->isKinematic) continue;

		for (auto& e2 : entity_manager->EntityFilter<ColliderComponent>().entities)
		{
			ColliderComponent* static_col = &e2->GetComponent<ColliderComponent>();
			if (static_col->isKinematic) continue;

			if (AABB(*kinematic_col, *static_col))
			{
				std::cout << e1->id << "hit" << e2->id << " | ";
			}
		}

		for (auto& e2 : entity_manager->EntityFilter<BoundaryComponent>().entities)
		{
			for (auto boundary_col : e2->GetComponent<BoundaryComponent>().boundaries)
			{
				if (boundary_col == nullptr) continue;
				if (AABB(*kinematic_col, *boundary_col))
				{
					std::cout << e1->id << "hit" << e2->id << " | ";
				}
			}
		}
	}
}

Vector2D CollisionSystem::ResolveOverlap(const ColliderComponent& rect1, const ColliderComponent& rect2)
{
	return Vector2D();
}

bool CollisionSystem::AABB(const ColliderComponent& col1, const ColliderComponent& col2)
{
	if (
		col1.position.x + col1.width / 2 >= col2.position.x - col2.width / 2 &&
		col2.position.x + col2.width / 2 >= col1.position.x - col1.width / 2 &&
		col1.position.y + col1.height / 2 >= col2.position.y - col2.height / 2 &&
		col2.position.y + col2.height / 2 >= col1.position.y - col1.height / 2
		)
	{
		return true;
	}

	return false;
}
