#include "ProjectileComponent.h"

ProjectileComponent::ProjectileComponent(Vector2Df start_pos, float rotation, float decay)
	: rotation(rotation), decay(decay), start_pos(start_pos)
{
}

void ProjectileComponent::SetHitScanRadius(float r)
{
	radius = r;
}
