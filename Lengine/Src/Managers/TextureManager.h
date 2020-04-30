#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include <memory>

class TextureManager
{
private:

	std::map<std::string, sf::Texture> textures;

public:

	void InitTextures();

	void AddTexture(std::string id, const char* path);
	sf::Texture& GetTexture(std::string id);
};