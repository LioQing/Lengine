#include "AIComponent.h"

AIComponent::AIComponent(int room) : room(room)
{
	movement.store(new Vector2Df(0.f, 0.f));
}

void AIComponent::Dead()
{
	movement.store(new Vector2Df(0.f, 0.f));
}
