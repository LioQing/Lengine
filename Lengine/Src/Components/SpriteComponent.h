#pragma once

#include <SFML/Graphics.hpp>
#include <Vector2D.h>
#include <lecs.h>

struct SpriteComponent : public lecs::Component
{
	sf::Sprite sprite;
	float draw_order_pt;

	SpriteComponent(std::string id);
	SpriteComponent(std::string id, Vector2Df origin);

	void SetDrawOrderPoint(float offset);
	float GetDrawOrderPoint();
};