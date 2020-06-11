#include "ProjHitBoxComponent.h"

ProjHitBoxComponent::ProjHitBoxComponent(float r) : radius(r)
{
}

ProjHitBoxComponent::ProjHitBoxComponent(float r, float y) : radius(r), y_offset(y)
{
}
