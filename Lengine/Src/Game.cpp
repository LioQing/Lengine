#include "Game.h"
#include "Components/Components.h"
#include "Systems/Systems.h"

Game::Game(sf::RenderWindow& window) : window(window)
{
	ecs_managers = lecs::ECSManagers();
	lecs::logger.AlwaysShow();
}

void Game::Init()
{
	texture_manager.InitTextures();

	ecs_managers.system_manager->AddSystem<MovementSystem>();
	ecs_managers.system_manager->AddSystem<SpriteSystem>();

	lecs::Entity* testEntity = &ecs_managers.entity_manager->AddEntity();
	SpriteComponent* sprite = &testEntity->AddComponent<SpriteComponent>("player", Vector2Df(16.0f, 16.0f));
	TransformComponent* transform = &testEntity->AddComponent<TransformComponent>(Vector2Df(400, 320), 32, 32, Vector2Df(3.0f, 3.0f), 2.4f);
	AnimationComponent* animation = &testEntity->AddComponent<AnimationComponent>();
	animation->AddAnimation("idle", 0, 2, 300);
	animation->AddAnimation("walk", 1, 8, 100);
	animation->SetCurrent("idle");
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
	ecs_managers.system_manager->Render(&window);
}
