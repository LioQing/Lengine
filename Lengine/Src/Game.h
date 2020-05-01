#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <lecs.h>

#include "Managers/TextureManager.h"
#include "Managers/CameraManager.h"

class Game
{
private:

	sf::RenderWindow& window;

public:

	lecs::ECSManagers ecs_managers;
	TextureManager texture_manager;
	CameraManager camera_manager;

	Game(sf::RenderWindow& window);

	void Init();
	void HandleInput(DeltaTime dt);
	void Update(DeltaTime dt);
	void Render();
};