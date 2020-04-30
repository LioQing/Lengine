#include "TransformComponent.h"

TransformComponent::TransformComponent(Vector2Df position) :
	position(position), speed(1.0f), velocity(Vector2Df(0.0f, 0.0f))
{
}

TransformComponent::TransformComponent(Vector2Df position, float speed) :
	position(position), speed(speed), velocity(Vector2Df(0.0f, 0.0f))
{
}

TransformComponent::TransformComponent(Vector2Df position, float speed, Vector2Df velocity) :
	position(position), speed(speed), velocity(velocity)
{
}
