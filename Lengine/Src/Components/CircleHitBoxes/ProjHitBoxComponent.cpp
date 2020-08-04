#include "ProjHitBoxComponent.h"

#include <cmath>

ProjHitBoxComponent::ProjHitBoxComponent(float r) : radius(r)
{
	InitCircle();
}

ProjHitBoxComponent::ProjHitBoxComponent(float r, float x_offset, float angle) : radius(r)
{
	InitCircle();
	offset = Vector2Df(x_offset * cosf(angle * M_PI / 180), x_offset * sinf(angle * M_PI / 180));
}

void ProjHitBoxComponent::InitCircle()
{
	circle.setRadius(this->radius);
	circle.setFillColor(sf::Color(sf::Color::Transparent));
	circle.setOutlineColor(sf::Color(sf::Color::Green));
	circle.setOrigin(circle.getRadius(), circle.getRadius());
	circle.setOutlineThickness(3.f);
}
