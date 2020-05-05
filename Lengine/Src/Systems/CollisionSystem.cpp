#include "CollisionSystem.h"

#include <vector>

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
		for (auto& boundary_col : e2->GetComponent<BoundaryComponent>().boundaries)
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
		
		std::vector<ColliderComponent*> collided;
		ColliderComponent tmp_resolved_col(*kinematic_col);
		Vector2Df resolve(0.0f, 0.0f);

		for (auto& e2 : entity_manager->EntityFilter<ColliderComponent>().entities)
		{
			ColliderComponent* static_col = &e2->GetComponent<ColliderComponent>();
			if (static_col->isKinematic) continue;

			if (AABB(tmp_resolved_col, *static_col))
			{
				collided.emplace_back(static_col);
				resolve = ApplyResolve(resolve, ResolveOverlap(tmp_resolved_col, *static_col));

				tmp_resolved_col.position = kinematic_col->position + resolve;
			}
		}

		for (auto& e2 : entity_manager->EntityFilter<BoundaryComponent>().entities)
		{
			for (auto boundary_col : e2->GetComponent<BoundaryComponent>().boundaries)
			{
				if (boundary_col == nullptr) continue;

				if (AABB(tmp_resolved_col, *boundary_col))
				{
					collided.emplace_back(boundary_col);
					resolve = ApplyResolve(resolve, ResolveOverlap(tmp_resolved_col, *boundary_col));

					tmp_resolved_col.position = kinematic_col->position + resolve;
				}
			}
		}

		tmp_resolved_col = *kinematic_col;
		Vector2Df reverse_resolve(0.0f, 0.0f);
		for (auto itr = collided.rbegin(); itr != collided.rend(); ++itr)
		{
			ColliderComponent* col = *itr;

			if (AABB(tmp_resolved_col, *col))
			{
				reverse_resolve = ApplyResolve(reverse_resolve, ResolveOverlap(tmp_resolved_col, *col));

				tmp_resolved_col.position = kinematic_col->position + reverse_resolve;
			}
		}

		if (!e1->HasComponent<TransformComponent>())
		{
			game->logger->AddLog
			(
				"Error: Entity " + std::to_string(e1->id) + " doesn't have Transform Component for collision resolve",
				lecs::LT_ERROR, lecs::LT_ENTITY, lecs::LT_COMPONENT
			);
			continue;
		}

		TransformComponent* transform = &e1->GetComponent<TransformComponent>();

		if (resolve.Magnitude() < reverse_resolve.Magnitude()) transform->position += resolve;
		else transform->position += reverse_resolve;

		if (kinematic_col->followTransform)
		{
			kinematic_col->position = transform->position;
		}
	}
}

Vector2Df CollisionSystem::ResolveOverlap(const ColliderComponent& kinematic_col, const ColliderComponent& static_col)
{
	Vector2D vMove = Vector2D(kinematic_col.position.x, kinematic_col.position.y);
	Vector2D vStatic = Vector2D(static_col.position.x, static_col.position.y);
	Vector2D diff = (vMove - vStatic) * Vector2D(kinematic_col.height + static_col.height, kinematic_col.width + static_col.width);

	if (fabs(diff.x) > fabs(diff.y))
	{
		if (diff.x > 0)
		{
			return Vector2Df(static_col.position.x + static_col.width * 0.5 - kinematic_col.position.x + kinematic_col.width * 0.5, 0);
		}
		else
		{
			return Vector2Df(-(kinematic_col.position.x + kinematic_col.width * 0.5 - static_col.position.x + static_col.width * 0.5), 0);
		}
	}
	else
	{
		if (diff.y > 0)
		{
			return Vector2Df(0, static_col.position.y + static_col.height * 0.5 - kinematic_col.position.y + kinematic_col.height * 0.5);
		}
		else
		{
			return Vector2Df(0, -(kinematic_col.position.y + kinematic_col.height * 0.5 - static_col.position.y + static_col.height * 0.5));
		}
	}
}

Vector2Df CollisionSystem::ApplyResolve(Vector2Df current_resolve, Vector2Df new_resolve)
{
	if (fabsf(new_resolve.x) > fabsf(current_resolve.x))
	{
		return Vector2Df(new_resolve.x, current_resolve.y);
	}
	else if (fabsf(new_resolve.y) > fabsf(current_resolve.y))
	{
		return Vector2Df(current_resolve.x, new_resolve.y);
	}
	return current_resolve;
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
