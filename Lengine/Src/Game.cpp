#include "Game.h"
#include "Components/SpriteComponent.h"

Game::Game(sf::RenderWindow& window) : window(window)
{
	ecs_managers = lecs::ECSManagers();
	lecs::logger.AlwaysShow();
}

void Game::Init()
{
	texture_manager.InitTextures();

	lecs::Entity* testEntity(&ecs_managers.entity_manager.AddEntity());
	SpriteComponent* sprite = &testEntity->AddComponent<SpriteComponent>();
	sprite->sprite.setTexture(texture_manager.GetTexture("player"));
	sprite->sprite.setScale(sf::Vector2f(3.0f, 3.0f));
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
	for (auto& e : ecs_managers.entity_manager.EntityFilter<SpriteComponent>().entities)
	{
		window.draw(e->GetComponent<SpriteComponent>().sprite);
	}
}
