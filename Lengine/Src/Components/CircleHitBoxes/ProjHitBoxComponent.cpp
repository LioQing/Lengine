#include "ProjHitBoxComponent.h"

ProjHitBoxComponent::ProjHitBoxComponent(float r) : radius(r)
{
	InitCircle();
}

ProjHitBoxComponent::ProjHitBoxComponent(float r, float x) : radius(r), x_offset(x)
{
	InitCircle();
}

void ProjHitBoxComponent::InitCircle()
{
	circle.setRadius(radius);
	circle.setFillColor(sf::Color(sf::Color::Transparent));
	circle.setOutlineColor(sf::Color(sf::Color::Green));
	circle.setOrigin(circle.getRadius(), circle.getRadius());
	circle.setOutlineThickness(3.f);
}
