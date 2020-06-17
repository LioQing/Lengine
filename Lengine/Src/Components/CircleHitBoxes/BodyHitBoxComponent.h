#pragma once

#include <SFML/Graphics.hpp>
#include <Vector2D.h>
#include <lecs.h>

struct BodyHitBoxComponent : public lecs::Component
{
	Vector2Df position;
	float radius;

	sf::CircleShape circle;

	BodyHitBoxComponent(Vector2Df position, float radius);
};