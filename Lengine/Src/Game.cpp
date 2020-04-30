#include "Game.h"
#include "Components/Components.h"
#include "Systems/MovementSystem.h"

Game::Game(sf::RenderWindow& window) : window(window)
{
	ecs_managers = lecs::ECSManagers();
	lecs::logger.AlwaysShow();
}

void Game::Init()
{
	texture_manager.InitTextures();

	ecs_managers.system_manager->AddSystem<MovementSystem>();

	lecs::Entity* testEntity = &ecs_managers.entity_manager->AddEntity();
	SpriteComponent* sprite = &testEntity->AddComponent<SpriteComponent>("player", 3.0f);
	TransformComponent* transform = &testEntity->AddComponent<TransformComponent>(Vector2Df(400, 320), 2.4f);
}

void Game::HandleInput(DeltaTime dt)
{
	ecs_managers.system_manager->HandleInput(dt);
}

void Game::Update(DeltaTime dt)
{
	ecs_managers.UpdateECSManagers(dt);

	ecs_managers.system_manager->GetSystem<MovementSystem>().LateUpdate(ecs_managers.entity_manager, ecs_managers.event_manager, dt);
}

void Game::Render()
{
	for (auto& e : ecs_managers.entity_manager->EntityFilter<SpriteComponent>().entities)
	{
		window.draw(e->GetComponent<SpriteComponent>().sprite);
	}
}
