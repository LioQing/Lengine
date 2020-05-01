#include "Game.h"
#include "Components/Components.h"
#include "Systems/Systems.h"

Game::Game(sf::RenderWindow& window) : window(window)
{
	ecs_managers = lecs::ECSManagers();
	camera_manager.SetWindow(&window);

	lecs::logger.AlwaysShow();
}

void Game::Init()
{
	texture_manager.InitTextures();

	ecs_managers.system_manager->AddSystem<TileMapSystem>();
	ecs_managers.system_manager->AddSystem<MovementSystem>();
	ecs_managers.system_manager->AddSystem<SpriteSystem>();

	lecs::Entity* map = &ecs_managers.entity_manager->AddEntity();
	TileMapComponent* tile_map = &map->AddComponent<TileMapComponent>("terrain", 32, 3);
	tile_map->LoadMap("Assets/Island.csv", 20, 20);

	lecs::Entity* player = &ecs_managers.entity_manager->AddEntity();
	SpriteComponent* sprite = &player->AddComponent<SpriteComponent>("player", Vector2Df(16.0f, 16.0f));
	TransformComponent* transform = &player->AddComponent<TransformComponent>(Vector2Df(400, 320), 32, 32, Vector2Df(3.0f, 3.0f), 2.4f);
	AnimationComponent* animation = &player->AddComponent<AnimationComponent>();
	animation->AddAnimation("idle", 0, 2, 300);
	animation->AddAnimation("walk", 1, 8, 100);

	camera_manager.SetFollow(&transform->position);
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
	camera_manager.Draw();
	window.setView(camera_manager.camera);
	ecs_managers.system_manager->Draw(&window);
}
