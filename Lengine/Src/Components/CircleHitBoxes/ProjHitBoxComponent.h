#pragma once

#include <lecs.h>

struct ProjHitBoxComponent : public lecs::Component
{
	float radius;
	float x_offset;

	ProjHitBoxComponent(float r);
	ProjHitBoxComponent(float r, float x);
};