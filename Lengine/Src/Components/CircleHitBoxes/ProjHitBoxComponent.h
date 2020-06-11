#pragma once

#include <lecs.h>

struct ProjHitBoxComponent : public lecs::Component
{
	float radius;
	float y_offset;

	ProjHitBoxComponent(float r);
	ProjHitBoxComponent(float r, float y);
};