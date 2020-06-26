#pragma once

#include <vector>
#include <lecs.h>
#include <thread>
#include <atomic>

#include "../Game.h"
#include "../Components/Components.h"

class Game;

class AIManager
{
private:

	std::vector<AIComponent*> enemies;

	std::thread ai_thread;
	std::atomic<bool> process_finished;

public:

	AIManager();

	void AddEnemy(AIComponent* c);

	void Read();

	void AIProcess();
};