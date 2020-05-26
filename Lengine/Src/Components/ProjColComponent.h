#pragma once

#include <Vector2D.h>
#include <lecs.h>

struct ProjColComponent : public lecs::Component
{
	sf::RectangleShape box;

	Vector2Df offset;
	Vector2Df position;
	int height;
	int width;

	bool fitTransform;
	bool followTransform;
	bool isKinematic;

	ProjColComponent(Vector2Df position, uint32_t height, uint32_t width);
	ProjColComponent(Vector2Df position, uint32_t height, uint32_t width, bool isKinematic);
	ProjColComponent(uint32_t height, uint32_t width, bool followTransform, bool isKinematic);
	ProjColComponent(bool fitTransform, bool followTransform, bool isKinematic);

	void SetOffset(Vector2Df offset);
	void SetPosition(Vector2Df pos);

	void UpdateBox();
};