#pragma once

#include <SFML/Graphics.hpp>
#include <lecs.h>

struct SpriteComponent : public lecs::Component
{
	sf::Sprite sprite;
};