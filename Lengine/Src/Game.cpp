#include "Game.h"

Game::Game(sf::RenderWindow& window) : window_(window)
{
	ecs_managers = lecs::ECSManagers();
}

void Game::HandleEvent()
{
}

void Game::Update(DeltaTime dt)
{
	ecs_managers.UpdateECSManagers(dt);
}

void Game::Render()
{
}
