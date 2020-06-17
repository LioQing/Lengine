#include "HealthSystem.h"

#include "../Game.h"
#include "../Components/Components.h"
#include "../Events/Events.h"

extern Game* game;

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
	}
}

void HealthSystem::Draw(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, sf::RenderWindow* window)
{
	for (auto& ch : entity_manager->EntityFilter<HealthComponent>().entities)
	{
		HealthComponent* hp = &ch->GetComponent<HealthComponent>();

		if (!game->InsideView(hp->bar_frame.getGlobalBounds())) continue;
		window->draw(hp->bar_frame);

		if (!game->InsideView(hp->bar.getGlobalBounds())) continue;
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
			hbev.hitbox->GetComponent<HealthComponent>().hp -= 2 + 2 * hbev.ishead;
		}
	}
}
