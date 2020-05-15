#include "BoundaryComponent.h"

#include <CSVReader.h>

BoundaryComponent::BoundaryComponent(const char* csv_path, uint32_t size_x, uint32_t size_y, uint32_t tile_size)
	: tile_size(tile_size)
{
	LoadBoundary(csv_path, size_x, size_y);
}

BoundaryComponent::BoundaryComponent(Matrixi map, int wall_i, int side_wall_i, uint32_t size_x, uint32_t size_y, uint32_t tile_size, Vector2Df wall_offset)
	: tile_size(tile_size)
{
	LoadBoundary(map, wall_i, side_wall_i, size_x, size_y, wall_offset);
}

void BoundaryComponent::LoadBoundary(Matrixi map, int wall_i, int side_wall_i, uint32_t size_x, uint32_t size_y, Vector2Df wall_offset)
{
	boundaries.resize(size_x * size_y);

	for (auto y = 0u; y < size_y; ++y)
	{
		for (auto x = 0u; x < size_x; ++x)
		{
			if (map.At(x, y) == wall_i)
			{
				boundaries[x + y * size_x] = new ColliderComponent(Vector2Df(x, y) * tile_size + tile_size / 2 + wall_offset, tile_size, tile_size);
			}
		}
	}
}

void BoundaryComponent::LoadBoundary(const char* csv_path, uint32_t size_x, uint32_t size_y)
{
	boundaries.resize(size_x * size_y);
	CSVReader csvr = CSVReader(csv_path, size_x, size_y);

	for (int y = 0; y < size_y; ++y)
	{
		for (int x = 0; x < size_x; ++x)
		{
			if (csvr.GetValue(y, x) == "0")
			{
				boundaries[x + y * size_x] = new ColliderComponent(Vector2Df(x, y) * tile_size + tile_size / 2, tile_size, tile_size);
			}
		}
	}
}