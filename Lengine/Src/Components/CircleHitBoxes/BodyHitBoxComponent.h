#pragma once

#include <Vector2D.h>
#include <lecs.h>

struct BodyHitBoxComponent : public lecs::Component
{
	Vector2Df position;
	float radius;

	BodyHitBoxComponent(Vector2Df position, float radius);
};