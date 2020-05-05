#pragma once

#include <SFML/Graphics.hpp>
#include <lecs.h>

struct SpriteSystem : public lecs::System
{
	void Update(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, DeltaTime dt) override;
	void LateUpdate(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, DeltaTime dt);
	void Draw(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, sf::RenderWindow* window) override;
};