#include "TransformComponent.h"

TransformComponent::TransformComponent(sf::Vector2f position) :
	position(position), speed(0.0f), velocity(sf::Vector2f(0.0f, 0.0f))
{
}

TransformComponent::TransformComponent(sf::Vector2f position, float speed) :
	position(position), speed(speed), velocity(sf::Vector2f(0.0f, 0.0f))
{
}

TransformComponent::TransformComponent(sf::Vector2f position, float speed, sf::Vector2f velocity) :
	position(position), speed(speed), velocity(velocity)
{
}
