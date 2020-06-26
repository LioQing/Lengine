#include "ProjColComponent.h"

#include "../Game.h"

extern std::atomic<Game*> game;

ProjColComponent::ProjColComponent(Vector2Df position, uint32_t height, uint32_t width)
	: position(position), height(height), width(width), fitTransform(false), followTransform(false), isKinematic(false)
{
}

ProjColComponent::ProjColComponent(Vector2Df position, uint32_t height, uint32_t width, bool isKinematic)
	: position(position), height(height), width(width), fitTransform(false), followTransform(false), isKinematic(isKinematic)
{
}

ProjColComponent::ProjColComponent(uint32_t height, uint32_t width, bool followTransform, bool isKinematic)
	: position(Vector2Df::Zero()), height(height), width(width), fitTransform(false), followTransform(followTransform), isKinematic(isKinematic)
{
}

ProjColComponent::ProjColComponent(bool fitTransform, bool followTransform, bool isKinematic)
	: position(Vector2Df::Zero()), height(0), width(0), fitTransform(fitTransform), followTransform(followTransform), isKinematic(isKinematic)
{
}

void ProjColComponent::SetOffset(Vector2Df offset)
{
	this->offset = offset;
}

void ProjColComponent::SetPosition(Vector2Df pos)
{
	position = pos + offset;
}

void ProjColComponent::UpdateBox()
{
	box.setOrigin(sf::Vector2f(width / 2 - 3, height / 2 - 3));
	box.setSize(sf::Vector2f(width - 6, height - 6));
	box.setPosition((position + offset).sfVector2f());
	box.setFillColor(sf::Color::Transparent);
	box.setOutlineColor(sf::Color(0, 255, 0, 191));
	box.setOutlineThickness(3);
}
