#pragma once

#include <lecs.h>
#include <vector>

#include "ColliderComponent.h"

struct BoundaryComponent : public lecs::Component
{
	std::vector<ColliderComponent*> boundaries;
	uint32_t tile_size;

	BoundaryComponent(const char* csv_path, uint32_t size_x, uint32_t size_y, uint32_t tile_size);
	void LoadBoundary(const char* csv_path, uint32_t size_x, uint32_t size_y);
};