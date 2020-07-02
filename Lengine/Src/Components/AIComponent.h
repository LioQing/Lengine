#pragma once

#include <lecs.h>
#include <cstdint>
#include <Vector2D.h>
#include <atomic>
#include <list>

struct AIComponent : lecs::Component
{
	std::atomic<Vector2Df*> movement;
	int room;

	std::list<Vector2Di> path;
	Vector2Di current_path;

	AIComponent(int room);
};