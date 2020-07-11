#include "ProjColSystem.h"

#include <vector>

#include "../Game.h"

extern std::atomic<Game*> game;

void ProjColSystem::Draw(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, sf::RenderWindow* window)
{
	for (auto& e : entity_manager->EntityFilter<ProjColComponent>().entities)
	{
		ProjColComponent* col = &e->GetComponent<ProjColComponent>();
		col->UpdateBox();
		window->draw(col->box);
	}
}

void ProjColSystem::Update(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, DeltaTime dt)
{
	for (auto& proj : entity_manager->EntityFilter<ProjectileComponent>().EntityFilter<TransformComponent>().entities)
	{
		float radius = proj->GetComponent<ProjectileComponent>().radius;

		for (auto& pc : entity_manager->EntityFilter<ProjColComponent>().entities)
		{
			if (HitBoxDetect(&pc->GetComponent<ProjColComponent>(), proj))
			{
				proj->Destroy();
				break;
			}
		}
	}
}

bool ProjColSystem::HitBoxDetect(ProjColComponent* col, lecs::Entity* e)
{
	auto pos = e->GetComponent<TransformComponent>().position + Vector2Df(e->GetComponent<TransformComponent>().width / 2, e->GetComponent<TransformComponent>().height / 2);
	if (
		pos.x < col->position.x + col->width / 2 &&
		pos.x > col->position.x - col->width / 2 &&
		pos.y < col->position.y + col->height / 2 &&
		pos.y > col->position.y - col->height / 2
		)
	{
		return true;
	}

	int n_check = 10;
	auto s_pos = e->GetComponent<ProjectileComponent>().start_pos;
	for (int m1 = 1; m1 < n_check; ++m1)
	{
		auto m2 = n_check - m1;
		auto c_pos = Vector2Df((m1 * s_pos.x + m2 * pos.x) / n_check, (m1 * s_pos.y + m2 * pos.y) / n_check);
		if (
			c_pos.x < col->position.x + col->width / 2 &&
			c_pos.x > col->position.x - col->width / 2 &&
			c_pos.y < col->position.y + col->height / 2 &&
			c_pos.y > col->position.y - col->height / 2
			)
		{
			return true;
		}
	}

	return false;
}