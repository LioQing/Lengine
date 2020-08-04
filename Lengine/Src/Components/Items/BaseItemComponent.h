#pragma once

#include <lecs.h>
#include <Vector2D.h>

class BaseItemComponent : public lecs::Component
{
public:

	bool isDropped;

	static const float equipped_dop;

	BaseItemComponent(bool isDropped);
};