#include "TextureManager.h"

#include <lecs.h>

void TextureManager::InitTextures()
{
	AddTexture("player", "Assets/player2.png");
	AddTexture("enemy", "Assets/enemy.png");
	AddTexture("terrain", "Assets/Dungeon_tileset2.png");
	AddTexture("glock", "Assets/csgo_glock.png");
	AddTexture("bullet", "Assets/bullet.png");
	AddTexture("crate", "Assets/static_objects/crate.png");
	AddTexture("d_crate", "Assets/static_objects/double_crate.png");
	AddTexture("stair_right", "Assets/static_objects/stair_right.png");
	AddTexture("stair_left", "Assets/static_objects/stair_left.png");
	AddTexture("stair_up", "Assets/static_objects/stair_up.png");
	AddTexture("stair_down", "Assets/static_objects/stair_down.png");
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
