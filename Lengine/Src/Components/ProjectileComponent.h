#pragma once

#include <Vector2D.h>
#include <lecs.h>

struct ProjectileComponent : public lecs::Component
{
	float radius;

	float rotation;

	float decay;
	float decay_timer = 0.f;

	ProjectileComponent(float rotation, float decay);

	void SetHitScanRadius(float r);
};