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

	bool debug_path = false;

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

	void Read(lecs::EntityManager* eman);
	void ReadMap();

	void StartProcess();
	void AIProcess();

private:

	const int min_idle = 200;
	const int max_idle = 1.5e3;

	AIComponent::STATE IdleActionSelect();
	void Walking(lecs::Entity* enemy);
	void IdleTime(lecs::Entity* enemy, DeltaTime dt);

	void ChasePlayerDest(AIComponent* e, TransformComponent* transform);
	void GenRandPath(AIComponent* e, TransformComponent* transform);
	void SolveAStar();
	bool CanSeePlayer(lecs::Entity* enemy);

	Node* nodes = nullptr;
	Node* n_start = nullptr;
	Node* n_end = nullptr;
	Matrixi map;
	std::vector<LevelComponent::Rect> rooms;

	// cross thread var
	std::atomic<Vector2Df*> player_pos = new Vector2Df(0.f, 0.f);
	std::atomic<bool> is_player_dead = false;
	std::atomic<Vector2Df*> player_vel = new Vector2Df(0.f, 0.f);
};