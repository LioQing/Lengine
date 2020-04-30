#pragma once

#include <SFML/Graphics.hpp>
#include <lecs.h>

struct TransformComponent : public lecs::Component
{
	sf::Vector2f position;
	sf::Vector2f velocity;
	float speed;

	TransformComponent(sf::Vector2f position);
	TransformComponent(sf::Vector2f position, float speed);
	TransformComponent(sf::Vector2f position, float speed, sf::Vector2f velocity);
};