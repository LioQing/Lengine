#include "GunComponent.h"

GunComponent::GunComponent(Vector2Df muzzle_pos, float fire_rate)
	: muzzle_pos(muzzle_pos), ready(true), fire_rate(fire_rate)
{
}
