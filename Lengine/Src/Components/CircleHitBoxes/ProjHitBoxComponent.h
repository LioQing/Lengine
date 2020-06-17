#pragma once

#include <SFML/Graphics.hpp>
#include <lecs.h>

struct ProjHitBoxComponent : public lecs::Component
{
	float radius;
	float x_offset;

	sf::CircleShape circle;

	ProjHitBoxComponent(float r);
	ProjHitBoxComponent(float r, float x);

	void InitCircle();
};