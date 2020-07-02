#pragma once

#include <vector>
#include <lecs.h>
#include <thread>
#include <atomic>
#include <Matrix.h>

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

	struct Node
	{
		bool isObstacle = false;
		bool isVisited = false;
		float h;
		float g;
		Vector2Di pos;
		std::vector<Node*> neighbours;
		Node* parent;
	};

	AIManager();

	void AddEnemy(AIComponent* c);

	void Read();
	void ReadMap();

	void StartProcess();
	void AIProcess();

	void SetDest(AIComponent* e, TransformComponent* transform);
	void SolveAStar();

private:

	Node* nodes = nullptr;
	Node* n_start = nullptr;
	Node* n_end = nullptr;
	Matrixi map;
	std::vector<LevelComponent::Rect> rooms;
};