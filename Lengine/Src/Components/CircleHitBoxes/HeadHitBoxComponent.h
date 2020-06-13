#pragma once

#include <Vector2D.h>
#include <lecs.h>

struct HeadHitBoxComponent : public lecs::Component
{
	Vector2Df position;
	float radius;

	HeadHitBoxComponent(Vector2Df position, float radius);
};