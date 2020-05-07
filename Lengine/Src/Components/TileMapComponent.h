#pragma once

#include <SFML/Graphics.hpp>
#include <lecs.h>
#include <Vector2D.h>
#include <vector>
#include <Matrix.h>

struct TileMapComponent : public lecs::Component
{
	std::vector<sf::Sprite> tiles;
	std::string texture_id;
	float scale;
	uint32_t tile_size;

	TileMapComponent(std::string texture_id, uint32_t tile_size, float scale);

	void SetTileSet(std::string texture_id, uint32_t tile_size, float scale);
	void LoadMap(const char* csv_path, uint32_t size_x, uint32_t size_y);
	void LoadMap(Matrixi map);

	void AddTile(int x, int y, int src_x, int src_y, sf::Sprite* tile);
};