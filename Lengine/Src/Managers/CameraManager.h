#pragma once

#include <SFML/Graphics.hpp>
#include <Vector2D.h>

class CameraManager
{
private:

	Vector2Df* follow;

public:

	sf::View camera;

	CameraManager() = default;

	void Draw(sf::RenderWindow* window);

	void SetCenter(Vector2Df center);
	void SetWindow(sf::RenderWindow* window);
	void SetFollow(Vector2Df* follow);
	sf::FloatRect GetViewBounds();
};