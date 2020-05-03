#pragma once

#include <SFML/Graphics.hpp>
#include <Vector2D.h>
#include <lecs.h>

struct ItemComponent : public lecs::Component
{
	sf::Sprite sprite;

	Vector2Df center;
	float radius;

	bool follow_transform;
	bool scale_transform;

	ItemComponent(std::string texture_id, bool follow_transform, bool scale_transform);

	void UpdateSprite();
};