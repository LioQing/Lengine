#include "Game.h"
#include "Components/Components.h"
#include "Systems/Systems.h"
#include "SpawnEntity.h"

Game::Game(sf::RenderWindow& window, ThreadPool& tp) : window(window), tp(tp)
{
	camera_manager.SetWindow(&window);

	logger = &lecs::logger;
	//lecs::logger.AlwaysShow();
}

void Game::Init()
{
	texture_manager.InitTextures();

	ecs_managers.system_manager->AddSystem<TileMapSystem>();
	ecs_managers.system_manager->AddSystem<MovementSystem>();
	ecs_managers.system_manager->AddSystem<SpriteSystem>();
	ecs_managers.system_manager->AddSystem<CollisionSystem>();
	ecs_managers.system_manager->AddSystem<ItemSystem>();
	ecs_managers.system_manager->AddSystem<ProjectileSystem>();

	lecs::Entity* map = spawn::Map();
	Vector2Df spawn_pos = Vector2Df(map->GetComponent<LevelComponent>().rooms.at(0).center.x, map->GetComponent<LevelComponent>().rooms.at(0).center.y) * world_scale * map->GetComponent<TileMapComponent>().tile_size;

	lecs::Entity* player = spawn::Player(spawn_pos);
	lecs::Entity* stair = spawn::Staircase(spawn_pos, "stair_up");

	camera_manager.SetFollow(&player->GetComponent<TransformComponent>().position);
}

void Game::HandleInput(DeltaTime dt)
{
	input_manager.HandleInput(&window);
	ecs_managers.system_manager->HandleInput(dt);

	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed) window.close();
		if (event.type == sf::Event::Resized) camera_manager.camera.setSize(event.size.width, event.size.height);
	}
}

void Game::Update(DeltaTime dt)
{
	ecs_managers.system_manager->GetSystem<MovementSystem>().EarlyUpdate(ecs_managers.entity_manager, ecs_managers.event_manager, dt);
	ecs_managers.system_manager->GetSystem<ItemSystem>().EarlyUpdate(ecs_managers.entity_manager, ecs_managers.event_manager, dt);
	ecs_managers.system_manager->GetSystem<ProjectileSystem>().EarlyUpdate(ecs_managers.entity_manager, ecs_managers.event_manager, dt);
	ecs_managers.system_manager->GetSystem<CollisionSystem>().EarlyUpdate(ecs_managers.entity_manager, ecs_managers.event_manager, dt);

	ecs_managers.UpdateECSManagers(dt);

	ecs_managers.system_manager->GetSystem<ItemSystem>().LateUpdate(ecs_managers.entity_manager, ecs_managers.event_manager, dt);
	ecs_managers.system_manager->GetSystem<MovementSystem>().LateUpdate(ecs_managers.entity_manager, ecs_managers.event_manager, dt);
	ecs_managers.system_manager->GetSystem<SpriteSystem>().LateUpdate(ecs_managers.entity_manager, ecs_managers.event_manager, dt);
}

void Game::Render()
{
	camera_manager.Draw(&window);
	ecs_managers.system_manager->Draw(&window);
	ecs_managers.system_manager->GetSystem<TileMapSystem>().LateDraw(ecs_managers.entity_manager, ecs_managers.event_manager, &window);
	ecs_managers.system_manager->GetSystem<CollisionSystem>().Draw(ecs_managers.entity_manager, ecs_managers.event_manager, &window);
}
