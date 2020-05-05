#pragma once

#include <Vector2D.h>
#include <lecs.h>

struct GunComponent : public lecs::Component
{
	bool ready;

	float fire_rate_timer = 0.f;
	float fire_rate; // delay in ms
	Vector2Df muzzle_pos;

	GunComponent(Vector2Df muzzle_pos, float fire_rate);
};