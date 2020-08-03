#include "AIComponent.h"

AIComponent::AIComponent(int room) : room(room)
{
	movement.store(new Vector2Df(1.f, 0.f));
	gun_pt_dir.store(new Vector2Df(1.f, 0.f));
	is_firing.store(false);
}

void AIComponent::Dead()
{
	*movement.load() = Vector2Df(0.f, 0.f);
}
