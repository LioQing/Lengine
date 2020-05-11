#pragma once

#include <lecs.h>
#include <vector>
#include <Matrix.h>

#include "ColliderComponent.h"

struct BoundaryComponent : public lecs::Component
{
	std::vector<ColliderComponent*> boundaries;
	uint32_t tile_size;

	BoundaryComponent(const char* csv_path, uint32_t size_x, uint32_t size_y, uint32_t tile_size);
	BoundaryComponent(Matrixi map, int wall_i, int side_wall_i, uint32_t size_x, uint32_t size_y, uint32_t tile_size, Vector2Df wall_offset = Vector2Df::Zero());

	void LoadBoundary(Matrixi map, int wall_i, int side_wall_i, uint32_t size_x, uint32_t size_y, Vector2Df wall_offset);
	void LoadBoundary(const char* csv_path, uint32_t size_x, uint32_t size_y);
};