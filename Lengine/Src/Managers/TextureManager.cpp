#include "TextureManager.h"

#include <lecs.h>

void TextureManager::InitTextures()
{
	AddTexture("player", "Assets/player.png");
	AddTexture("terrain", "Assets/TileSet_Export.png");
}

void TextureManager::AddTexture(std::string id, const char* path)
{
	sf::Texture texture;
	if (!texture.loadFromFile(path))
	{
		lecs::logger.AddLog("Error: Cannot load texture from file", lecs::LT_ERROR);
	}
	textures[id] = texture;
}

sf::Texture& TextureManager::GetTexture(std::string id)
{
	return textures.at(id);
}
