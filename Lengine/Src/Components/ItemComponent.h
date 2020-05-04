#pragma once

#include <SFML/Graphics.hpp>
#include <Vector2D.h>
#include <lecs.h>

struct ItemComponent : public lecs::Component
{
	sf::Sprite sprite;

	Vector2Df center;
	Vector2Df scale;
	float radius;

	bool follow_transform;
	bool scale_transform;

	ItemComponent(std::string texture_id, bool follow_transform, bool scale_transform);
	ItemComponent(std::string texture_id, float radius, bool follow_transform, bool scale_transform);
	ItemComponent(std::string texture_id, Vector2Df center, float radius, bool scale_transform);
	ItemComponent(std::string texture_id, float radius, Vector2Df scale, bool follow_transform);
	ItemComponent(std::string texture_id, Vector2Df center, float radius, Vector2Df scale);

	void SetSprite(std::string texture_id);
	void UpdateSprite(Vector2Df position = Vector2Df::Right());
};