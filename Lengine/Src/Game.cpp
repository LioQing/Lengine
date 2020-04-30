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
	TransformComponent* transform = &testEntity->AddComponent<TransformComponent>(sf::Vector2f(400, 320));
}

void Game::HandleEvent()
{
}

void Game::Update(DeltaTime dt)
{
	ecs_managers.UpdateECSManagers(dt);
}

void Game::Render()
{
	//std::cout << ecs_managers.entity_manager.EntityFilter<SpriteComponent>().entities.size() << std::endl;
	for (auto& e : ecs_managers.entity_manager->EntityFilter<SpriteComponent>().entities)
	{
		window.draw(e->GetComponent<SpriteComponent>().sprite);
	}
}
