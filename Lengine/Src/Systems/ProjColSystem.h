#pragma once

#include <Vector2D.h>
#include <lecs.h>

#include "../Components/Components.h"

struct ProjColSystem : public lecs::System
{
	bool draw;

	void Update(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, DeltaTime dt) override;
	void Draw(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, sf::RenderWindow* window) override;

	bool HitBoxDetect(ProjColComponent* col, lecs::Entity* e);
};