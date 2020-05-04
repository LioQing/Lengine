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

		transform->velocity = Vector2Df(cosf(projectile->rotation), sinf(projectile->rotation));
	}
}
