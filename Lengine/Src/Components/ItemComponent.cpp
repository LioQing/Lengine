#include "ItemComponent.h"

#include "../Game.h"

extern Game* game;

ItemComponent::ItemComponent(std::string texture_id, bool follow_transform, bool scale_transform)
	: center(Vector2Df(0.0f, 0.0f)), radius(0.0f), follow_transform(follow_transform), scale_transform(scale_transform)
{
	sprite.setTexture(game->texture_manager.GetTexture(texture_id));
	sprite.setOrigin(sf::Vector2f(0.0f, 0.0f));
}

void ItemComponent::UpdateSprite()
{
	sprite.setPosition(center.sfVector2f());
}
