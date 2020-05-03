#include "InputManager.h"

#include "../Game.h"

extern Game* game;

void InputManager::HandleInput(sf::RenderWindow* window)
{
	world_mouse_pos = Vector2D::ToVector2D<float>(window->mapPixelToCoords(sf::Mouse::getPosition(*window), game->camera_manager.camera));
}
