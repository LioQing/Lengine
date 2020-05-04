#pragma once

#include <lecs.h>

struct ProjectileComponent : public lecs::Component
{
	float rotation;

	ProjectileComponent(float rotation);
};