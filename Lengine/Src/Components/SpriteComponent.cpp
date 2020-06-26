#include "SpriteComponent.h"

#include "../Game.h"

extern std::atomic<Game*> game;

SpriteComponent::SpriteComponent(std::string id)
{
	sprite.setTexture(game.load()->texture_manager.GetTexture(id));
	sprite.setOrigin(sprite.getTextureRect().width / 2, sprite.getTextureRect().height / 2);
	SetDrawOrderPoint(0.f);
}

SpriteComponent::SpriteComponent(std::string id, Vector2Df origin)
{
	sprite.setTexture(game.load()->texture_manager.GetTexture(id));
	sprite.setOrigin(origin.sfVector2f());
	SetDrawOrderPoint(0.f);
}

void SpriteComponent::SetDrawOrderPoint(float offset)
{
	draw_order_pt = offset;
}

float SpriteComponent::GetDrawOrderPoint()
{
	return sprite.getGlobalBounds().top + sprite.getGlobalBounds().height + draw_order_pt;
}
