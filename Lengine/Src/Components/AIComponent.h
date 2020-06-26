#pragma once

#include <lecs.h>
#include <cstdint>
#include <Vector2D.h>
#include <atomic>

struct AIComponent : lecs::Component
{
	std::atomic<Vector2Di*> movement;

	AIComponent();
};