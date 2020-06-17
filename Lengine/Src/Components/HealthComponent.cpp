#include "HealthComponent.h"

HealthComponent::HealthComponent(float max) : hp(100.f), max_hp(max)
{
	bar.setFillColor(sf::Color::Green);
	bar_frame.setFillColor(sf::Color::Transparent);
	bar_frame.setOutlineColor(sf::Color::Black);
	bar_frame.setOutlineThickness(2.f);
}

void HealthComponent::SetHealthBar(Vector2Df off)
{
	offset = off;
}
