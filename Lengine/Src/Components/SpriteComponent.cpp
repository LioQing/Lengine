#include "SpriteComponent.h"

#include "../Game.h"

extern Game* game;

SpriteComponent::SpriteComponent(std::string id)
{
	sprite.setTexture(game->texture_manager.GetTexture(id));
	sprite.setOrigin(16, 16);
}

SpriteComponent::SpriteComponent(std::string id, Vector2Df origin)
{
	sprite.setTexture(game->texture_manager.GetTexture(id));
	sprite.setOrigin(origin.sfVector2f());
}
