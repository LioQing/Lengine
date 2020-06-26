#include "AIManager.h"

extern std::atomic<Game*> game;

AIManager::AIManager()
{
	process_finished = true;

	ai_thread = std::thread(&AIManager::AIProcess, this);
}

void AIManager::AddEnemy(uint32_t id)
{
	enemies.emplace_back(new AIHandle(id));
}

void AIManager::Read()
{
	process_finished = false;
}

void AIManager::AIProcess()
{
	for (;;) std::cout << process_finished << std::endl;
}
