#include "ItemComponent.h"

#include <cmath>

#include "SpriteComponent.h"
#include "TransformComponent.h"
#include "../Game.h"

extern std::atomic<Game*> game;

ItemComponent::ItemComponent(lecs::Entity* item, bool follow_transform)
	: center(Vector2Df(0.0f, 0.0f)), radius(0.0f), follow_transform(follow_transform)
{
	SetItem(item);
}

ItemComponent::ItemComponent(lecs::Entity* item, float radius, bool follow_transform)
	: center(Vector2Df(0.0f, 0.0f)), radius(radius), follow_transform(follow_transform)
{
	SetItem(item);
}

ItemComponent::ItemComponent(lecs::Entity* item, Vector2Df center, float radius)
	: center(center), radius(radius), follow_transform(false)
{
	SetItem(item);
}

void ItemComponent::SetItem(lecs::Entity* item)
{
	this->item = item;
}

void ItemComponent::RenderSprite(Vector2Df position)
{
	float rad = atan2f(position.y, position.x);
	item->GetComponent<TransformComponent>().position = center + Vector2Df(radius * cosf(rad), radius * sinf(rad));
	if (position.x < 0) item->GetComponent<TransformComponent>().scale.y = fabsf(item->GetComponent<TransformComponent>().scale.y) * -1;
	else if (position.x > 0) item->GetComponent<TransformComponent>().scale.y = fabsf(item->GetComponent<TransformComponent>().scale.y);

	item->GetComponent<SpriteComponent>().sprite.setRotation(rad * 180 / M_PI);
}