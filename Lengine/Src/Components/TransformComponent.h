#pragma once

#include <Vector2D.h>
#include <lecs.h>

struct TransformComponent : public lecs::Component
{
	Vector2Df position;
	Vector2Df velocity;
	float speed;

	TransformComponent(Vector2Df position);
	TransformComponent(Vector2Df position, float speed);
	TransformComponent(Vector2Df position, float speed, Vector2Df velocity);
};