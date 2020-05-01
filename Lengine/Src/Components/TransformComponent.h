#pragma once

#include <Vector2D.h>
#include <lecs.h>

struct TransformComponent : public lecs::Component
{
	Vector2Df position;
	uint32_t width;
	uint32_t height;
	Vector2Df scale;

	Vector2Df velocity;
	float speed;

	TransformComponent(Vector2Df position, uint32_t width, uint32_t height);
	TransformComponent(Vector2Df position, uint32_t width, uint32_t height, Vector2Df scale);
	TransformComponent(Vector2Df position, uint32_t width, uint32_t height, Vector2Df scale, float speed);
	TransformComponent(Vector2Df position, uint32_t width, uint32_t height, Vector2Df scale, float speed, Vector2Df velocity);
};