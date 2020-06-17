#include "HealthSystem.h"

#include "../Components/Components.h"
#include "../Events/Events.h"

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

		Vector2Df size(36.f, 4.f); // default size

		hp->bar.setSize(Vector2Df(size.x * hp->hp / hp->max_hp, size.y).sfVector2f());
		hp->bar.setOrigin(0.f, size.y / 2);
		hp->bar.setPosition((transform->position + hp->offset - Vector2Df(size.x / 2, 0.f)).sfVector2f());

		hp->bar_frame.setSize(size.sfVector2f());
		hp->bar_frame.setOrigin(0.f, size.y / 2);
		hp->bar_frame.setPosition((transform->position + hp->offset - Vector2Df(size.x / 2, 0.f)).sfVector2f());
	}
}

void HealthSystem::Draw(lecs::EntityManager* entity_manager, lecs::EventManager* event_manager, sf::RenderWindow* window)
{
	for (auto& ch : entity_manager->EntityFilter<HealthComponent>().entities)
	{
		HealthComponent* hp = &ch->GetComponent<HealthComponent>();
		window->draw(hp->bar);
		window->draw(hp->bar_frame);
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
