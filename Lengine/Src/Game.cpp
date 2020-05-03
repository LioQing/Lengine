#include "Game.h"
#include "Components/Components.h"
#include "Systems/Systems.h"

Game::Game(sf::RenderWindow& window) : window(window)
{
	camera_manager.SetWindow(&window);

	logger = &lecs::logger;
	lecs::logger.AlwaysShow();
}

void Game::Init()
{
	texture_manager.InitTextures();

	ecs_managers.system_manager->AddSystem<TileMapSystem>();
	ecs_managers.system_manager->AddSystem<MovementSystem>();
	ecs_managers.system_manager->AddSystem<SpriteSystem>();
	ecs_managers.system_manager->AddSystem<CollisionSystem>();
	ecs_managers.system_manager->AddSystem<ItemSystem>();

	lecs::GetComponentTypeID<ItemComponent>(); // strange bug in lecs... have to fix it like this

	lecs::Entity* map = &ecs_managers.entity_manager->AddEntity();
	BoundaryComponent* boundary = &map->AddComponent<BoundaryComponent>("Assets/Boundary.csv", 20, 20, 96);
	TileMapComponent* tilemap = &map->AddComponent<TileMapComponent>("terrain", 32, 3);
	tilemap->LoadMap("Assets/Island.csv", 20, 20);

	lecs::Entity* player = &ecs_managers.entity_manager->AddEntity();
	ItemComponent* item = &player->AddComponent<ItemComponent>("glock", true, true);
	SpriteComponent* sprite = &player->AddComponent<SpriteComponent>("player", Vector2Df(16.0f, 16.0f));
	TransformComponent* transform = &player->AddComponent<TransformComponent>(Vector2Df(400, 320), 32, 32, Vector2Df(3.0f, 3.0f), 2.4f);
	ColliderComponent* collider = &player->AddComponent<ColliderComponent>(96, 48, true, true);
	AnimationComponent* animation = &player->AddComponent<AnimationComponent>();
	animation->AddAnimation("idle", 0, 2, 300);
	animation->AddAnimation("walk", 1, 8, 100);
	ecs_managers.entity_manager->AddToGroup(player, lecs::GRP_PLAYER);

	camera_manager.SetFollow(&transform->position);
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
	ecs_managers.system_manager->GetSystem<CollisionSystem>().EarlyUpdate(ecs_managers.entity_manager, ecs_managers.event_manager, dt);

	ecs_managers.UpdateECSManagers(dt);

	ecs_managers.system_manager->GetSystem<MovementSystem>().LateUpdate(ecs_managers.entity_manager, ecs_managers.event_manager, dt);
}

void Game::Render()
{
	camera_manager.Draw(&window);
	ecs_managers.system_manager->Draw(&window);
	ecs_managers.system_manager->GetSystem<ItemSystem>().Draw(ecs_managers.entity_manager, ecs_managers.event_manager, &window);
}
