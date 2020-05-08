#include "LevelComponent.h"

#include <time.h>

LevelComponent::LevelComponent(int null_i)
	: null_index(null_i)
{
	srand(time(NULL));
}

void LevelComponent::GenMap(uint32_t n_room, uint32_t map_size, uint32_t room_min, uint32_t room_max, int floor_i, int wall_i, int side_wall_i, float cor_r, float cor_r_backup)
{
	floor_index = floor_i;
	wall_index = wall_i;
	side_wall_index = side_wall_i;

	map = Matrixi(map_size, map_size, null_index);
	uint32_t edge = (room_max + room_min) / 2;

	for (auto i = 0u; i < n_room; ++i)
	{
		Rect r1 = GenRect(Vector2Di(Random(edge, map_size - edge), Random(edge, map_size - edge)),
			Random(room_min, room_max), Random(room_min, room_max));
		bool flag = false;

		for (auto& r : rooms)
		{
			if (AABB(r1, r))
			{
				flag = true;
				--i;
				break;
			}
		}

		if (!flag)
		{
			rooms.push_back(r1);
			map.Rectangle(r1.x, r1.y, r1.width, r1.height, floor_i);
		}
	}

	for (auto& r1 : rooms)
	{
		bool hasCor = false;

		for (auto& r2 : rooms)
		{
			if (r1.center == r2.center) continue;

			if (
				(Vector2Di(r1.x, r1.y) - r2.center).Magnitude() < cor_r * (r1.width + r1.height) ||
				(Vector2Di(r1.x + r1.width, r1.y) - r2.center).Magnitude() < cor_r * (r1.width + r1.height) ||
				(Vector2Di(r1.x, r1.y + r1.height) - r2.center).Magnitude() < cor_r * (r1.width + r1.height) ||
				(Vector2Di(r1.x + r1.width, r1.y + r1.height) - r2.center).Magnitude() < cor_r * (r1.width + r1.height)
				)
			{
				hasCor = true;

				map.Line(r1.center.x, r1.center.y, r2.center.x, r1.center.y, floor_i);
				map.Line(r2.center.x, r1.center.y, r2.center.x, r2.center.y, floor_i);
			}
		}

		if (!hasCor)
		{
			for (auto& r2 : rooms)
			{
				if (r1.center == r2.center) continue;

				if (
					(Vector2Di(r1.x, r1.y) - r2.center).Magnitude() < cor_r_backup * (r1.width + r1.height) ||
					(Vector2Di(r1.x + r1.width, r1.y) - r2.center).Magnitude() < cor_r_backup * (r1.width + r1.height) ||
					(Vector2Di(r1.x, r1.y + r1.height) - r2.center).Magnitude() < cor_r_backup * (r1.width + r1.height) ||
					(Vector2Di(r1.x + r1.width, r1.y + r1.height) - r2.center).Magnitude() < cor_r_backup * (r1.width + r1.height)
					)
				{
					hasCor = true;

					map.Line(r1.center.x, r1.center.y, r2.center.x, r1.center.y, floor_i);
					map.Line(r2.center.x, r1.center.y, r2.center.x, r2.center.y, floor_i);
				}
			}
		}
	}

	BuildWall(floor_index, null_index, wall_index, side_wall_index);
}

void LevelComponent::BuildWall(int floor_i, int null_i, int wall_i, int side_wall_i)
{
	for (auto h = 0u; h < map.height; ++h)
	{
		for (auto w = 0u; w < map.width; ++w)
		{
			if (map.At(w, h) == floor_i) continue;

			for (auto x = w - 1; x <= w + 1; ++x)
			{
				for (auto y = h - 1; y <= h + 1; ++y)
				{
					if (
						(x == w && y == h) ||
						x >= map.width ||
						x < 0 ||
						y >= map.height ||
						y < 0
						)
						continue;

					if (map.At(x, y) == floor_i)
					{
						map.At(w, h) = wall_i;
						goto wall_built;
					}
				}
			}

		wall_built:;
		}
	}

	for (auto h = 0u; h < map.height; ++h)
	{
		for (auto w = 0u; w < map.width; ++w)
		{
			if (map.At(w, h) != wall_i) continue;

			if (h != map.height - 1 && map.At(w, h + 1) == floor_i)
			{
				map.At(w, h) = side_wall_i;
				if (h != 0)
				{
					if (map.At(w, h - 1) == floor_i && (h - 1 == 0 || map.At(w, h - 2) == wall_i || map.At(w, h - 2) == side_wall_i))
					{
						map.At(w, h) = floor_i;
						map.At(w, h - 1) = floor_i;
					}
					else
					{
						map.At(w, h - 1) = wall_i;
					}
				}

				if (w != 0 && map.At(w - 1, h - 1) == null_i) map.At(w - 1, h - 1) = wall_i;
				if (w != map.width - 1 && map.At(w + 1, h - 1) == null_i) map.At(w + 1, h - 1) = wall_i;
			}
		}
	}
}

LevelComponent::Rect LevelComponent::GenRect(Vector2Di center, uint32_t width, uint32_t height)
{
	Rect rect;
	rect.center = center;
	rect.x = center.x - width / 2;
	rect.y = center.y - height / 2;
	rect.width = width;
	rect.height = height;

	return rect;
}

bool LevelComponent::AABB(Rect r1, Rect r2)
{
	if (
		r1.x + r1.width >= r2.x &&
		r2.x + r2.width >= r1.x &&
		r1.y + r1.height >= r2.y &&
		r2.y + r2.height >= r1.y
		)
	{
		return true;
	}

	return false;
}

int LevelComponent::Random(int lower, int upper)
{
	return rand() % (upper - lower + 1) + lower;
}
