#pragma once

#include <SFML/Graphics.hpp>
#include <Vector2D.h>
#include <lecs.h>

struct ItemComponent : public lecs::Component
{
	lecs::Entity* item;

	Vector2Df center;
	float radius;

	bool follow_transform;
	bool scale_transform;

	ItemComponent(lecs::Entity* item, bool follow_transform);
	ItemComponent(lecs::Entity* item, float radius, bool follow_transform);
	ItemComponent(lecs::Entity* item, Vector2Df center, float radius);

	void SetItem(lecs::Entity* item);

	void RenderSprite(Vector2Df position = Vector2Df::Right());
};