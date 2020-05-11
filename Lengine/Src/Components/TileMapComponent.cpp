#include "TileMapComponent.h"

#include <CSVReader.h>

#include "../Game.h"

extern Game* game;

TileMapComponent::TileMapComponent(std::string texture_id, uint32_t tile_size, float scale)
{
	SetTileSet(texture_id, tile_size, scale);
}

void TileMapComponent::SetTileSet(std::string texture_id, uint32_t tile_size, float scale)
{
	this->texture_id = texture_id;
	this->tile_size = tile_size;
	this->scale = scale;
}

void TileMapComponent::LoadMap(const char* csv_path, uint32_t size_x, uint32_t size_y)
{
	tiles.resize(size_x * size_y);
	top_layer_tiles.resize(size_x * size_y);
	CSVReader csvr = CSVReader(csv_path, size_y, size_x);

	for (int y = 0; y < size_y; ++y)
	{
		for (int x = 0; x < size_x; ++x)
		{
			int src_x = (csvr.GetValue<int>(y, x) % 10) * tile_size;
			int src_y = (csvr.GetValue<int>(y, x) / 10) * tile_size;

			sf::Sprite* tile;
			if (csvr.GetValue<int>(y, x) == top_layer_i) tile = &top_layer_tiles[x + y * size_x];
			else tile = &tiles[x + y * size_x];
			AddTile(x, y, src_x, src_y, tile);
		}
	}
}

void TileMapComponent::LoadMap(Matrixi map)
{
	tiles.resize(map.width * map.height);
	top_layer_tiles.resize(map.width * map.height);

	for (int y = 0; y < map.height; ++y)
	{
		for (int x = 0; x < map.width; ++x)
		{
			if (map.At(x, y) == -1) continue;
			int src_x = (map.At(x, y) % 10) * tile_size;
			int src_y = (map.At(x, y) / 10) * tile_size;

			sf::Sprite* tile;
			if (map.At(x, y) == top_layer_i) tile = &top_layer_tiles[x + y * map.width];
			else tile = &tiles[x + y * map.width];
			AddTile(x, y, src_x, src_y, tile);
		}
	}
}

void TileMapComponent::AddTile(int x, int y, int src_x, int src_y, sf::Sprite* tile)
{
	tile->setPosition(x * tile_size * scale, y * tile_size * scale);
	tile->setScale(scale + 0.005f, scale + 0.005f); // I hate this fix...
	tile->setTexture(game->texture_manager.GetTexture(texture_id));
	tile->setTextureRect(sf::IntRect(src_x, src_y, tile_size, tile_size));
}