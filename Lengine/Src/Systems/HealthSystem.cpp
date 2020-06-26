#include "HealthSystem.h"

#include "../Game.h"
#include "../Components/Components.h"
#include "../Events/Events.h"

extern std::atomic<Game*> game;

void HealthSystem::Init(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, lecs::SystemManager* system_manager)
{
	event_manager->Subscribe<HitBoxEvent>(this);
}

void HealthSystem::Update(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, DeltaTime dt)
{
	for (auto& ch : entity_manager->EntityFilter<HealthComponent>().entities)
	{
		HealthComponent* hp = &ch->GetComponent<HealthComponent>();
		TransformComponent* transform = &ch->GetComponent<TransformComponent>();

		hp->bar.setPosition((transform->position + hp->offset - Vector2Df(hp->size.x / 2, 0.f)).sfVector2f());
		hp->bar.setSize(Vector2Df(hp->size.x * hp->hp / hp->max_hp, hp->size.y).sfVector2f());
		hp->bar_frame.setPosition((transform->position + hp->offset - Vector2Df(hp->size.x / 2, 0.f)).sfVector2f());

		if (hp->is_dead && ch->HasComponent<AnimationComponent>())
		{
			AnimationComponent* anim = &ch->GetComponent<AnimationComponent>();
			
			if (anim->animations.count("dead"))
			{
				anim->SetCurrent("dead");
			}
		}
	}
}

void HealthSystem::Draw(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, sf::RenderWindow* window)
{
	for (auto& ch : entity_manager->EntityFilter<HealthComponent>().entities)
	{
		HealthComponent* hp = &ch->GetComponent<HealthComponent>();

		if (hp->is_dead) continue;

		if (!game.load()->InsideView(hp->bar_frame.getGlobalBounds())) continue;
		window->draw(hp->bar_frame);

		if (!game.load()->InsideView(hp->bar.getGlobalBounds())) continue;
		window->draw(hp->bar);
	}
}

void HealthSystem::Receive(lecs::Event& event)
{
	if (event.IsEvent<HitBoxEvent>())
	{
		HitBoxEvent hbev = event.Downcast<HitBoxEvent>();
		
		if (hbev.hitbox->HasComponent<HealthComponent>())
		{
			HealthComponent* hp = &hbev.hitbox->GetComponent<HealthComponent>();

			hp->TakeDamage(2 + 2 * hbev.ishead);
		}
	}
}
