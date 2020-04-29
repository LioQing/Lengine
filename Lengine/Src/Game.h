#pragma once

#include <lecs.h>

class Game
{
private:

public:

	lecs::ECSManagers ecs_managers;

	Game()
	{
		ecs_managers = lecs::ECSManagers();
	}

	void Update(DeltaTime dt)
	{
		ecs_managers.UpdateECSManagers(dt);
	}
};