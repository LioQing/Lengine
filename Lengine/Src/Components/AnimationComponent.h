#pragma once

#include <lecs.h>
#include <map>

struct AnimationComponent : public lecs::Component
{
	struct Animation
	{
		uint32_t index;
		uint32_t frame;
		uint32_t delay;

		Animation& operator=(const Animation& anim)
		{
			index = anim.index;
			frame = anim.frame;
			delay = anim.delay;

			return *this;
		}

		Animation() = default;
		Animation(uint32_t i, uint32_t f, uint32_t d) : index(i), frame(f), delay(d) {}
	};

	DeltaTime timer;
	Animation current;
	std::map<std::string, Animation> animations;

	AnimationComponent() = default;

	void SetCurrent(std::string id);
	void AddAnimation(std::string id, uint32_t i, uint32_t f, uint32_t d);
};