#pragma once

#include <lecs.h>
#include <Vector2D.h>
#include <SFML/Graphics.hpp>

struct HealthComponent : public lecs::Component
{
	float max_hp;
	float hp;

	bool is_dead = false;

	const Vector2Df size = { 36.f, 4.f }; // default size

	Vector2Df offset = Vector2Df::Zero();

	sf::RectangleShape bar;
	sf::RectangleShape bar_frame;

	HealthComponent(float max);

	void SetHealthBar(Vector2Df off);
	void TakeDamage(float dmg);
};