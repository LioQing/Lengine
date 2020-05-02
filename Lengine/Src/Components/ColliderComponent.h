#pragma once

#include <SFML/Graphics.hpp>
#include <cstdint>
#include <lecs.h>
#include <Vector2D.h>

struct ColliderComponent : public lecs::Component
{
	sf::RectangleShape box;

	Vector2Df position;
	int height;
	int width;

	bool fitTransform;
	bool followTransform;
	bool isKinematic;

	ColliderComponent(Vector2Df position, uint32_t height, uint32_t width);
	ColliderComponent(Vector2Df position, uint32_t height, uint32_t width, bool isKinematic);
	ColliderComponent(uint32_t height, uint32_t width, bool followTransform, bool isKinematic);
	ColliderComponent(bool fitTransform, bool followTransform, bool isKinematic);

	void UpdateBox();
};