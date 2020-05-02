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
	CSVReader csvr = CSVReader(csv_path, size_y, size_x);

	for (int y = 0; y < size_y; ++y)
	{
		for (int x = 0; x < size_x; ++x)
		{
			int src_x = (csvr.GetValue<int>(y, x) % 10) * tile_size;
			int src_y = (csvr.GetValue<int>(y, x) / 10) * tile_size;

			sf::Sprite* tile = &tiles[x + y * size_x];

			tile->setPosition(x * tile_size * scale, y * tile_size * scale);
			tile->setScale(scale + 0.005f, scale + 0.005f); // I hate this fix...
			tile->setTexture(game->texture_manager.GetTexture(texture_id));
			tile->setTextureRect(sf::IntRect(src_x, src_y, tile_size, tile_size));
		}
	}
}
