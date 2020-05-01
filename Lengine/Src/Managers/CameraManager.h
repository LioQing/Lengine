#pragma once

#include <SFML/Graphics.hpp>
#include <Vector2D.h>

class CameraManager
{
private:

	Vector2Df* follow;
	sf::RenderWindow* window;

public:

	sf::View camera;

	CameraManager() = default;

	void Draw();

	void SetCenter(Vector2Df center);
	void SetWindow(sf::RenderWindow* window);
	void SetFollow(Vector2Df* follow);
	sf::FloatRect GetViewBounds();
};