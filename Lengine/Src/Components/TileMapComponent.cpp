#include "TileMapComponent.h"

#include <CSVReader.h>

#include "../Game.h"

extern Game* game;

TileMapComponent::TileMapComponent(std::string texture_id, uint32_t tile_size, float scale)
{
	tiles.setPrimitiveType(sf::Quads);
	SetTileSet(texture_id, tile_size, scale);
}

void TileMapComponent::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	states.texture = &game->texture_manager.GetTexture(texture_id);

	target.draw(tiles, states);
}

void TileMapComponent::SetTileSet(std::string texture_id, uint32_t tile_size, float scale)
{
	this->texture_id = texture_id;
	this->tile_size = tile_size;
	this->scale = scale;
}

void TileMapComponent::LoadMap(const char* csv_path, uint32_t size_x, uint32_t size_y)
{
	tiles.resize(size_x * size_y * 4);
	CSVReader csvr = CSVReader(csv_path, size_y, size_x);

	for (int y = 0; y < size_y; ++y)
	{
		for (int x = 0; x < size_x; ++x)
		{
			int src_x = (csvr.GetValue<int>(y, x) % 10) * tile_size;
			int src_y = (csvr.GetValue<int>(y, x) / 10) * tile_size;

			sf::Vertex* tile = &tiles[(x + y * size_x) * 4];

			tile[0].position = sf::Vector2f(x * tile_size * scale, y * tile_size * scale);
			tile[1].position = sf::Vector2f((x + 1) * tile_size * scale, y * tile_size * scale);
			tile[2].position = sf::Vector2f((x + 1) * tile_size * scale, (y + 1) * tile_size * scale);
			tile[3].position = sf::Vector2f(x * tile_size * scale, (y + 1) * tile_size * scale);

			tile[0].texCoords = sf::Vector2f(src_x, src_y);
			tile[1].texCoords = sf::Vector2f(src_x + tile_size, src_y);
			tile[2].texCoords = sf::Vector2f(src_x + tile_size, src_y + tile_size);
			tile[3].texCoords = sf::Vector2f(src_x, src_y + tile_size);
		}
	}
	std::cout << std::endl;
}
