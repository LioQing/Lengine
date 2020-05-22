#include "ProjectileSystem.h"

#include "../Game.h"
#include "../Components/Components.h"

extern Game* game;

void ProjectileSystem::EarlyUpdate(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, DeltaTime delta_time)
{
	for (auto& e : entity_manager->EntityFilter<ProjectileComponent>().entities)
	{
		if (!e->HasComponent<TransformComponent>())
		{
			game->logger->AddLog
			(
				"Error: Entity " + std::to_string(e->id) + " doesn't have Transform Component for Projectile Component",
				lecs::LT_ENTITY, lecs::LT_COMPONENT, lecs::LT_ERROR
			);
			continue;
		}

		TransformComponent* transform = &e->GetComponent<TransformComponent>();
		ProjectileComponent* projectile = &e->GetComponent<ProjectileComponent>();

		transform->velocity = Vector2Df(cosf(projectile->rotation * M_PI / 180), sinf(projectile->rotation * M_PI / 180));
	}
}

void ProjectileSystem::Update(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, DeltaTime delta_time)
{
	for (auto& e : entity_manager->EntityFilter<ProjectileComponent>().entities)
	{
		ProjectileComponent* projectile = &e->GetComponent<ProjectileComponent>();

		projectile->decay_timer += delta_time;
		if (projectile->decay_timer > projectile->decay)
		{
			e->Destroy();
			continue;
		}

		std::vector<bool> results;

		for (auto& c : entity_manager->EntityFilter<ColliderComponent>().entities)
		{
			if ((e->GetComponent<TransformComponent>().position - c->GetComponent<ColliderComponent>().position).Magnitude() > 100) continue;
			results.emplace_back(HitWallDetect(&c->GetComponent<ColliderComponent>(), e));
		}
		for (auto& e2 : entity_manager->EntityFilter<BoundaryComponent>().entities)
		{
			for (auto* boundary_col : e2->GetComponent<BoundaryComponent>().boundaries)
			{
				if (boundary_col == nullptr || 
					(e->GetComponent<TransformComponent>().position - boundary_col->position).Magnitude() > 100) continue;
				results.emplace_back(HitWallDetect(boundary_col, e));
			}
		}

		for (bool r : results)
		{
			if (r)
			{
				e->Destroy();
				break;
			}
		}
	}
}

bool ProjectileSystem::HitWallDetect(ColliderComponent* col, lecs::Entity* e)
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
	return false;
}
