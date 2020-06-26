#pragma once

#include <cstdint>
#include <Vector2D.h>

struct AIHandle
{
	uint32_t id;

	Vector2Di movement;

	AIHandle(uint32_t);
};