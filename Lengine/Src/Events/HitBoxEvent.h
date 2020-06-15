#pragma once

#include <lecs.h>

struct HitBoxEvent : public lecs::Event
{
	lecs::Entity* projectile;
	lecs::Entity* hitbox;
	bool ishead;

	HitBoxEvent() = default;
	HitBoxEvent(lecs::Entity* proj, lecs::Entity* hitbox, bool ishead) : projectile(proj), hitbox(hitbox), ishead(ishead) {}
};