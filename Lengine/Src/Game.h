#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <lecs.h>

#include "Managers/TextureManager.h"

class Game
{
private:

	sf::RenderWindow& window;

public:

	lecs::ECSManagers ecs_managers;
	TextureManager texture_manager;

	Game(sf::RenderWindow& window);

	void Init();
	void HandleEvent();
	void Update(DeltaTime dt);
	void Render();
};