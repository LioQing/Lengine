#pragma once

#include <vector>
#include <memory>
#include <lecs.h>

class InputHandler
{
public:

	virtual void HandleInput(lecs::EntityManager* entity_manager) {}
};

class InputManager
{
private:

	lecs::EntityManager* entity_manager;
	std::vector<std::unique_ptr<InputHandler>> handlers;

public:

	InputManager(lecs::EntityManager* entity_manager);

	void Update();
};