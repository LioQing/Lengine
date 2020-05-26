#include "ProjectileComponent.h"

ProjectileComponent::ProjectileComponent(float rotation, float decay)
	: rotation(rotation), decay(decay)
{
}

void ProjectileComponent::SetHitScanRadius(float r)
{
	radius = r;
}
