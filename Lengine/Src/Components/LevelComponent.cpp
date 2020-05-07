#include "LevelComponent.h"

#include <time.h>

LevelComponent::LevelComponent(int null_i)
	: null_index(null_i)
{
	srand(time(NULL));
}

void LevelComponent::GenMap(uint32_t n_room, uint32_t map_size, uint32_t room_min, uint32_t room_max, int floor_i, float cor_r, float cor_r_backup)
{
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
