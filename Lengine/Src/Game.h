#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <lecs.h>

class Game
{
private:

	sf::RenderWindow& window_;

public:

	lecs::ECSManagers ecs_managers;

	Game(sf::RenderWindow& window);

	void HandleEvent();
	void Update(DeltaTime dt);
	void Render();
};