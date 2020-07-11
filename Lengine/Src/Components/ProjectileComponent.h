#pragma once

#include <Vector2D.h>
#include <lecs.h>

struct ProjectileComponent : public lecs::Component
{
	float radius;

	float rotation;

	float decay;
	float decay_timer = 0.f;

	Vector2Df start_pos;

	ProjectileComponent(Vector2Df start_pos, float rotation, float decay);

	void SetHitScanRadius(float r);
};