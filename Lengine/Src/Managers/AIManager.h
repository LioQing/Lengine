#pragma once

#include <vector>
#include <lecs.h>
#include <thread>
#include <atomic>

#include "../Game.h"
#include "AIHandle.h"

class Game;

class AIManager
{
private:

	std::vector<std::shared_ptr<AIHandle>> enemies;

	std::thread ai_thread;
	std::atomic<bool> process_finished;

public:

	AIManager();

	void AddEnemy(uint32_t id);

	void Read();

	void AIProcess();
};