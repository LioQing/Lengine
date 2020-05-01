#pragma once

#include <SFML/Graphics.hpp>
#include <lecs.h>
#include <Vector2D.h>

class TileMapComponent : public lecs::Component, public sf::Drawable, public sf::Transformable
{
private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

public:

	sf::VertexArray tiles;
	std::string texture_id;
	float scale;
	uint32_t tile_size;

	TileMapComponent(std::string texture_id, uint32_t tile_size, float scale);

	void SetTileSet(std::string texture_id, uint32_t tile_size, float scale);
	void LoadMap(const char* csv_path, uint32_t size_x, uint32_t size_y);
};