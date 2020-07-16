#pragma once

#include <lecs.h>
#include <cstdint>
#include <Vector2D.h>
#include <atomic>
#include <list>

struct AIComponent : lecs::Component
{
	enum STATE
	{
		NONE,
		IDLE,
		IDLE_WALKING,

		SIZE
	};

	// cross thread variable
	std::atomic<Vector2Df*> movement;
	std::atomic<Vector2Df*> gun_pt_dir;

	// ai processor only
	STATE state = STATE::NONE;
	int room;

	std::list<Vector2Di> path;
	Vector2Di current_path;
	Vector2Di* ended_path = nullptr;

	float timer = -1.f;
	int idle_time_threshold;

	AIComponent(int room);

	void Dead();
};