#include "ItemComponent.h"

#include <cmath>

#include "../Game.h"

extern Game* game;

ItemComponent::ItemComponent(std::string texture_id, bool follow_transform, bool scale_transform)
	: center(Vector2Df(0.0f, 0.0f)), radius(0.0f), follow_transform(follow_transform), scale_transform(scale_transform)
{
	SetSprite(texture_id);
}

ItemComponent::ItemComponent(std::string texture_id, float radius, bool follow_transform, bool scale_transform)
	: center(Vector2Df(0.0f, 0.0f)), radius(radius), follow_transform(follow_transform), scale_transform(scale_transform)
{
	SetSprite(texture_id);
}

ItemComponent::ItemComponent(std::string texture_id, Vector2Df center, float radius, bool scale_transform)
	: center(center), radius(radius), follow_transform(false), scale_transform(scale_transform)
{
	SetSprite(texture_id);
}

ItemComponent::ItemComponent(std::string texture_id, float radius, Vector2Df scale, bool follow_transform)
	: center(Vector2Df(0.0f, 0.0f)), radius(radius), scale(scale), follow_transform(follow_transform), scale_transform(false)
{
	SetSprite(texture_id);
}

ItemComponent::ItemComponent(std::string texture_id, Vector2Df center, float radius, Vector2Df scale)
	: center(center), radius(radius), scale(scale), follow_transform(false), scale_transform(false)
{
	SetSprite(texture_id);
}

void ItemComponent::SetSprite(std::string texture_id)
{
	sprite.setTexture(game->texture_manager.GetTexture(texture_id));
	sprite.setOrigin(sf::Vector2f(sprite.getTexture()->getSize().x / 2, sprite.getTexture()->getSize().y / 2));
}

void ItemComponent::UpdateSprite()
{
	Vector2Df relative_mouse_pos = game->input_manager.world_mouse_pos - center;
	float mouse_rad = atan2f(relative_mouse_pos.y, relative_mouse_pos.x);

	sprite.setPosition((center + Vector2Df(radius * cosf(mouse_rad), radius * sinf(mouse_rad))).sfVector2f());
	if (relative_mouse_pos.x < 0) scale.y = fabsf(scale.y) * -1;
	sprite.setScale(scale.sfVector2f());
	sprite.setRotation(mouse_rad * 180 / M_PI);
}
