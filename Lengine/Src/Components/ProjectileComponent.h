#pragma once

#include <lecs.h>

struct ProjectileComponent : public lecs::Component
{
	float rotation;

	float decay;
	float decay_timer = 0.f;

	ProjectileComponent(float rotation, float decay);
};