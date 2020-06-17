#pragma once

#include <SFML/Graphics.hpp>
#include <Vector2D.h>
#include <lecs.h>

struct HeadHitBoxComponent : public lecs::Component
{
	Vector2Df position;
	float radius;

	sf::CircleShape circle;

	HeadHitBoxComponent(Vector2Df position, float radius);
};