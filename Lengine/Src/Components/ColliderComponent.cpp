#include "ColliderComponent.h"

#include "../Game.h"

extern Game* game;

ColliderComponent::ColliderComponent(Vector2Df position, uint32_t height, uint32_t width, TAG tag)
	: position(position), height(height), width(width), fitTransform(false), followTransform(false), isKinematic(false), tag(tag)
{
}

ColliderComponent::ColliderComponent(Vector2Df position, uint32_t height, uint32_t width, bool isKinematic, TAG tag)
	: position(position), height(height), width(width), fitTransform(false), followTransform(false), isKinematic(isKinematic), tag(tag)
{
}

ColliderComponent::ColliderComponent(uint32_t height, uint32_t width, bool followTransform, bool isKinematic, TAG tag)
	: position(Vector2Df::Zero()), height(height), width(width), fitTransform(false), followTransform(followTransform), isKinematic(isKinematic), tag(tag)
{
}

ColliderComponent::ColliderComponent(bool fitTransform, bool followTransform, bool isKinematic, TAG tag)
	: position(Vector2Df::Zero()), height(0), width(0), fitTransform(fitTransform), followTransform(followTransform), isKinematic(isKinematic), tag(tag)
{
}

void ColliderComponent::SetOffset(Vector2Df offset)
{
	this->offset = offset;
}

void ColliderComponent::SetPosition(Vector2Df pos)
{
	position = pos + offset;
}

void ColliderComponent::UpdateBox()
{
	box.setOrigin(sf::Vector2f(width / 2 - 3, height / 2 - 3));
	box.setSize(sf::Vector2f(width - 6, height - 6));
	box.setPosition((position + offset).sfVector2f());
	box.setFillColor(sf::Color::Transparent);
	box.setOutlineColor(sf::Color(0, 255, 0, 191));
	box.setOutlineThickness(3);
}
