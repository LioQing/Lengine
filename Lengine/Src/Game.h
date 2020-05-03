#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <lecs.h>

#include "Managers/Managers.h"

class Game
{
private:

	sf::Event event;
	sf::RenderWindow& window;

public:

	lecs::ECSManagers ecs_managers;
	lecs::Logger* logger;

	TextureManager texture_manager;
	CameraManager camera_manager;
	InputManager input_manager;

	Game(sf::RenderWindow& window);

	void Init();
	void HandleInput(DeltaTime dt);
	void Update(DeltaTime dt);
	void Render();
};