#pragma once

#include <SFML/Graphics.hpp>
#include <Vector2D.h>

class InputManager
{
public:

	Vector2Df world_mouse_pos;

	void HandleInput(sf::RenderWindow* window);
};