#pragma once

#include <Vector2D.h>
#include <lecs.h>

#include "../Components/Components.h"

struct CollisionSystem : public lecs::System
{
	void EarlyUpdate(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, DeltaTime dt);
	void Update(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, DeltaTime dt) override;
	void Draw(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, sf::RenderWindow* window) override;

	Vector2D ResolveOverlap(const ColliderComponent& rect1, const ColliderComponent& rect2);

	bool AABB(const ColliderComponent& col1, const ColliderComponent& col2);
};