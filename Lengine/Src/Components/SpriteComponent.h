#pragma once

#include <SFML/Graphics.hpp>
#include <lecs.h>

struct SpriteComponent : public lecs::Component
{
	sf::Sprite sprite;

	SpriteComponent(std::string id);
	SpriteComponent(std::string id, float scale);
};