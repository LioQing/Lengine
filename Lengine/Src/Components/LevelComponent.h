#pragma once

#include <Vector2D.h>
#include <lecs.h>
#include <Matrix.h>
#include <vector>
#include <initializer_list>

struct LevelComponent : public lecs::Component
{
	struct Rect
	{
		int x, y;
		uint32_t width, height;
		Vector2Di center;

		Rect() = default;
		Rect(int x, int y, uint32_t w, uint32_t h) : x(x), y(y), width(w), height(h)
		{
			center.x = x + w / 2;
			center.y = y + h / 2;
		}
	};

	Matrixi map;
	Matrixi static_layer;
	std::vector<Rect> rooms;
	std::vector<std::string> statics;

	int null_index;
	int floor_index;
	int wall_index;
	int side_wall_index;

	LevelComponent(int null_i);

	void GenMap(uint32_t n_room, uint32_t map_size, uint32_t room_min, uint32_t room_max, int floor_i, int wall_i, int side_wall_i, float cor_r = .8f, float cor_r_backup = 1.5f);
	void BuildWall(int floor_i, int null_i, int wall_i, int side_wall_i);
	void GenStatics(int lower_num, int upper_num, int tile_size, lecs::EntityManager* entity_manager);
	void SetStaticTextures(std::initializer_list<std::string> ids);

	Rect GenRect(Vector2Di center, uint32_t width, uint32_t height);
	bool AABB(Rect r1, Rect r2);
	int Random(int lower, int upper);
};