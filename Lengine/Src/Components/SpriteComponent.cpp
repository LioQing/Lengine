#include "SpriteComponent.h"

#include "../Game.h"

extern Game* game;

SpriteComponent::SpriteComponent(std::string id)
{
	sprite.setTexture(game->texture_manager.GetTexture(id));
}

SpriteComponent::SpriteComponent(std::string id, float scale)
{
	sprite.setTexture(game->texture_manager.GetTexture(id));
	sprite.setScale(sf::Vector2f(scale, scale));
}
