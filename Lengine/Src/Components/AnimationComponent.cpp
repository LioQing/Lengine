#include "AnimationComponent.h"

void AnimationComponent::SetCurrent(std::string id)
{
	current = animations.at(id);
}

void AnimationComponent::AddAnimation(std::string id, uint32_t i, uint32_t f, uint32_t d)
{
	Animation anim = Animation(i, f, d);
	if (current.delay == 0) current = anim;
	animations.emplace(id, anim);
}
