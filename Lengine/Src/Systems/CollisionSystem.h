#pragma once

#include <Vector2D.h>
#include <lecs.h>

#include "../Components/Components.h"

struct CollisionSystem : public lecs::System
{
	void EarlyUpdate(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, DeltaTime dt);
	void Update(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, DeltaTime dt) override;
	void Draw(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, sf::RenderWindow* window) override;

	void LateDraw(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, sf::RenderWindow* window);
	void SubUpdate(lecs::Entity* e1, lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, DeltaTime dt);

	Vector2Df ResolveOverlap(const ColliderComponent& kinematic_col, const ColliderComponent& static_col);
	Vector2Df ApplyResolve(Vector2Df current_resolve, Vector2Df new_resolve);

	bool AABB(const ColliderComponent& col1, const ColliderComponent& col2);
};