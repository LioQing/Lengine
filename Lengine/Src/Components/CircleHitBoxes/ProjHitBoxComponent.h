#pragma once

#include <Vector2D.h>
#include <SFML/Graphics.hpp>
#include <lecs.h>

struct ProjHitBoxComponent : public lecs::Component
{
	float radius;
	Vector2Df offset;

	sf::CircleShape circle;

	ProjHitBoxComponent(float r);
	ProjHitBoxComponent(float r, float x, float angle);

	void InitCircle();
};