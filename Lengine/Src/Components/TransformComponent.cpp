#include "TransformComponent.h"

TransformComponent::TransformComponent(Vector2Df position, uint32_t width, uint32_t height) 
	: position(position), width(width), height(height), scale(Vector2Df(1.0f, 1.0f)), speed(1.0f), velocity(Vector2Df(0.0f, 0.0f))
{
}

TransformComponent::TransformComponent(Vector2Df position, uint32_t width, uint32_t height, Vector2Df scale) 
	: position(position), width(width), height(height), scale(scale), speed(1.0f), velocity(Vector2Df(0.0f, 0.0f))
{
}

TransformComponent::TransformComponent(Vector2Df position, uint32_t width, uint32_t height, Vector2Df scale, float speed) 
	: position(position), width(width), height(height), scale(scale), speed(speed), velocity(Vector2Df(0.0f, 0.0f))
{
}

TransformComponent::TransformComponent(Vector2Df position, uint32_t width, uint32_t height, Vector2Df scale, float speed, Vector2Df velocity)
	: position(position), width(width), height(height), scale(scale), speed(speed), velocity(velocity)
{
}
