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

void SpriteSystem::Draw(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, sf::RenderWindow* window)
{
	for (auto& e : entity_manager->EntityFilter<TransformComponent>().EntityFilter<SpriteComponent>().entities)
	{
		TransformComponent* transform = &e->GetComponent<TransformComponent>();
		SpriteComponent* sprite = &e->GetComponent<SpriteComponent>();

		// off screen no draw
		if (
			sprite->sprite.getGlobalBounds().left > game->camera_manager.GetViewBounds().left + game->camera_manager.GetViewBounds().width ||
			sprite->sprite.getGlobalBounds().top > game->camera_manager.GetViewBounds().top + game->camera_manager.GetViewBounds().height ||
			sprite->sprite.getGlobalBounds().left + sprite->sprite.getGlobalBounds().width < game->camera_manager.GetViewBounds().left ||
			sprite->sprite.getGlobalBounds().top + sprite->sprite.getGlobalBounds().height < game->camera_manager.GetViewBounds().top
			)
		{
			continue;
		}

		// scale
		sprite->sprite.setScale(transform->scale.sfVector2f());

		// animation
		if (e->HasComponent<AnimationComponent>())
		{
			AnimationComponent* animation = &e->GetComponent<AnimationComponent>();
			sf::IntRect srcRect;
			srcRect.width = transform->width;
			srcRect.height = transform->height;
			srcRect.left = srcRect.width * (static_cast<int>(animation->timer / animation->current.delay) % animation->current.frame);
			srcRect.top = animation->current.index * transform->height;

			sprite->sprite.setTextureRect(srcRect);
		}

		// draw
		window->draw(sprite->sprite);
	}
}
