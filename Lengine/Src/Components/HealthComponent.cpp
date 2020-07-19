#include "HealthComponent.h"

HealthComponent::HealthComponent(float max) : hp(max), max_hp(max)
{
	bar.setFillColor(sf::Color::Green);
	bar.setOrigin(0.f, size.y / 2);

	bar_frame.setSize(size.sfVector2f());
	bar_frame.setOrigin(0.f, size.y / 2);
	bar_frame.setFillColor(sf::Color::Transparent);
	bar_frame.setOutlineColor(sf::Color::Black);
	bar_frame.setOutlineThickness(2.f);
}

void HealthComponent::SetHealthBar(Vector2Df off)
{
	offset = off;
}

void HealthComponent::TakeDamage(float dmg)
{
	hp -= dmg;

	if (hp <= 0)
	{
		hp = 0.f;
		is_dead = true;
	}
}
