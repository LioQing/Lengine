#include "SpriteSystem.h"

#include "../Game.h"
#include "../Components/Components.h"

extern Game* game;

void SpriteSystem::Update(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, DeltaTime dt)
{
	for (auto& e : entity_manager->EntityFilter<AnimationComponent>().entities)
	{
		AnimationComponent* animation = &e->GetComponent<AnimationComponent>();

		animation->timer += dt;
		animation->timer = fmodf(animation->timer, animation->current.frame * animation->current.delay);
	}
}

void SpriteSystem::LateUpdate(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, DeltaTime dt)
{
	for (auto& e : entity_manager->EntityFilter<TransformComponent>().EntityFilter<SpriteComponent>().entities)
	{
		TransformComponent* transform = &e->GetComponent<TransformComponent>();
		SpriteComponent* sprite = &e->GetComponent<SpriteComponent>();

		// scale
		sprite->sprite.setScale(transform->scale.sfVector2f());

		// animation
		if (e->HasComponent<AnimationComponent>())
		{
			AnimationComponent* animation = &e->GetComponent<AnimationComponent>();
			sf::IntRect srcRect;
			srcRect.width = transform->width;
			srcRect.height = transform->height;
			if (animation->current.delay != 0)
			{
				srcRect.left = srcRect.width * (static_cast<int>(animation->timer / animation->current.delay) % animation->current.frame);
			}
			else
			{
				game->logger->AddLog
				(
					"Error: Animation Component current animation delay is 0, cannot divide by 0",
					lecs::LT_ERROR, lecs::LT_COMPONENT
				);
			}
			srcRect.top = animation->current.index * transform->height;

			sprite->sprite.setTextureRect(srcRect);
		}
	}
}

void SpriteSystem::Draw(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, sf::RenderWindow* window)
{
	for (int grp = lecs::GRP_RENDER_BEGIN; grp != lecs::GRP_RENDER_END; ++grp)
	{
		std::vector<SpriteComponent*> draw_order;

		for (auto& e : entity_manager->GetGroup(grp).EntityFilter<TransformComponent>().EntityFilter<SpriteComponent>().entities)
		{
			TransformComponent* transform = &e->GetComponent<TransformComponent>();
			SpriteComponent* sprite = &e->GetComponent<SpriteComponent>();

			// off screen no draw
			if (!game->InsideView(sprite->sprite.getGlobalBounds())) continue;

			// insert
			draw_order.push_back(sprite);
		}

		for (int x = 0; x < draw_order.size(); x++)
		{
			for (int y = 0; y < draw_order.size() - 1; y++)
			{
				if (draw_order[y]->GetDrawOrderPoint() > draw_order[y + 1]->GetDrawOrderPoint())
				{
					SpriteComponent* temp = draw_order[y + 1];
					draw_order[y + 1] = draw_order[y];
					draw_order[y] = temp;
				}
			}
		}

		for (auto s : draw_order) window->draw(s->sprite);
	}
}