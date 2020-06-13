#include "ProjHitBoxComponent.h"

ProjHitBoxComponent::ProjHitBoxComponent(float r) : radius(r)
{
}

ProjHitBoxComponent::ProjHitBoxComponent(float r, float x) : radius(r), x_offset(x)
{
}
