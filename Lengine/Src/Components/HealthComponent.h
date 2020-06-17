#pragma once

#include <lecs.h>
#include <Vector2D.h>
#include <SFML/Graphics.hpp>

struct HealthComponent : public lecs::Component
{
	float max_hp;
	float hp;

	Vector2Df offset = Vector2Df::Zero();

	sf::RectangleShape bar;
	sf::RectangleShape bar_frame;

	HealthComponent(float max);

	void SetHealthBar(Vector2Df off);
};