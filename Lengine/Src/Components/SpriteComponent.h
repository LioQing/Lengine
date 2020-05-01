#pragma once

#include <SFML/Graphics.hpp>
#include <Vector2D.h>
#include <lecs.h>

struct SpriteComponent : public lecs::Component
{
	sf::Sprite sprite;

	SpriteComponent(std::string id);
	SpriteComponent(std::string id, Vector2Df origin);
};