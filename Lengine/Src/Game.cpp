#include "Game.h"
#include "Components/Components.h"
#include "Systems/Systems.h"
#include "Events/Events.h"
#include "SpawnEntity.h"

#include <memory>
lecs::Entity* player;
Game::Game(sf::RenderWindow& window) : window(window)
{
	ai_manager = new AIManager();

	camera_manager.SetWindow(&window);

	logger = &lecs::logger;
	//lecs::logger.AlwaysShow();
}

void Game::Init()
{
	texture_manager.InitTextures();

	// events
	ecs_managers.event_manager->AddEvent<HitBoxEvent>();

	// systems
	ecs_managers.system_manager->AddSystem<TileMapSystem>();
	ecs_managers.system_manager->AddSystem<MovementSystem>();
	ecs_managers.system_manager->AddSystem<SpriteSystem>();
	ecs_managers.system_manager->AddSystem<CollisionSystem>();
	ecs_managers.system_manager->AddSystem<ItemSystem>();
	ecs_managers.system_manager->AddSystem<ProjectileSystem>();
	ecs_managers.system_manager->AddSystem<ProjColSystem>();
	ecs_managers.system_manager->AddSystem<HitBoxSystem>();
	ecs_managers.system_manager->AddSystem<HealthSystem>();
	ecs_managers.system_manager->AddSystem<AIMovementSystem>();

	// spawns
	lecs::Entity* map = spawn::Map();
	Vector2Df spawn_pos = Vector2Df(map->GetComponent<LevelComponent>().rooms.at(0).center.x, map->GetComponent<LevelComponent>().rooms.at(0).center.y) * world_scale * map->GetComponent<TileMapComponent>().tile_size;

	player = spawn::Player(spawn_pos);
	lecs::Entity* enemy = spawn::Enemy(spawn_pos, 0);

	// camera
	camera_manager.SetFollow(&player->GetComponent<TransformComponent>().position);

	// other things for debugging
	//ecs_managers.system_manager->GetSystem<CollisionSystem>().draw_colBox = true;
	//ecs_managers.system_manager->GetSystem<ProjColSystem>().draw = true;
	//HitBoxSystem* hb_system = &ecs_managers.system_manager->GetSystem<HitBoxSystem>();
	//hb_system->draw_characters = true;
	//hb_system->draw_proj = true;

	// read map
	ai_manager->ReadMap();
	ai_manager->StartProcess();
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
	//std::cout << "P: " << player->GetComponent<TransformComponent>().position.x << ", " << player->GetComponent<TransformComponent>().position.y << " ";
	ecs_managers.system_manager->GetSystem<MovementSystem>().EarlyUpdate(ecs_managers.entity_manager, ecs_managers.event_manager, dt);
	ecs_managers.system_manager->GetSystem<ItemSystem>().EarlyUpdate(ecs_managers.entity_manager, ecs_managers.event_manager, dt);
	ecs_managers.system_manager->GetSystem<ProjectileSystem>().EarlyUpdate(ecs_managers.entity_manager, ecs_managers.event_manager, dt);
	ecs_managers.system_manager->GetSystem<HitBoxSystem>().EarlyUpdate(ecs_managers.entity_manager, ecs_managers.event_manager, dt);
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
	ecs_managers.system_manager->GetSystem<ProjColSystem>().Draw(ecs_managers.entity_manager, ecs_managers.event_manager, &window);

	ai_manager->Read();
}

bool Game::InsideView(sf::FloatRect rect)
{
	if (rect.left > camera_manager.GetViewBounds().left + camera_manager.GetViewBounds().width ||
		rect.top > camera_manager.GetViewBounds().top + camera_manager.GetViewBounds().height ||
		rect.left + rect.width < camera_manager.GetViewBounds().left ||
		rect.top + rect.height < camera_manager.GetViewBounds().top)
		return false;
	return true;
}
