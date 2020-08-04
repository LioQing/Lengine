#include "BaseItemComponent.h"

const float BaseItemComponent::equipped_dop{ 12.f };

BaseItemComponent::BaseItemComponent(bool isDropped)
	: isDropped(isDropped)
{
}
