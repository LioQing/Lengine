#include "AIManager.h"

extern std::atomic<Game*> game;

AIManager::AIManager()
{
	process_finished = true;

	ai_thread = std::thread(&AIManager::AIProcess, this);
}

void AIManager::AddEnemy(AIComponent* c)
{
	enemies.emplace_back(c);
}

void AIManager::Read()
{
	process_finished = false;
}

void AIManager::AIProcess()
{
	for (;;)
	{
		while (process_finished == true) {}

		for (auto& e : enemies)
		{
			e->movement.store(new Vector2Di(rand() % 3 - 1, rand() % 3 - 1));
			std::cout << e->movement.load()->x << ", " << e->movement.load()->y << std::endl;
		}

		process_finished = true;
	}
}
