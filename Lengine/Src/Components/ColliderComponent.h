#pragma once

#include <SFML/Graphics.hpp>
#include <cstdint>
#include <lecs.h>
#include <Vector2D.h>

struct ColliderComponent : public lecs::Component
{
	enum TAG
	{
		SIDE_WALL,
		PLAYER,
		STATIC,
		WALL,
		ENEMY,

		TAG_SIZE
	};

	TAG tag;
	sf::RectangleShape box;

	Vector2Df offset;
	Vector2Df position;
	int height;
	int width;

	bool fitTransform;
	bool followTransform;
	bool isKinematic;

	ColliderComponent(Vector2Df position, uint32_t height, uint32_t width, TAG tag);
	ColliderComponent(Vector2Df position, uint32_t height, uint32_t width, bool isKinematic, TAG tag);
	ColliderComponent(uint32_t height, uint32_t width, bool followTransform, bool isKinematic, TAG tag);
	ColliderComponent(bool fitTransform, bool followTransform, bool isKinematic, TAG tag);

	void SetOffset(Vector2Df offset);
	void SetPosition(Vector2Df pos);

	void InitBox();
};