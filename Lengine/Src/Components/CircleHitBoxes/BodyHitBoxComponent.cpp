#include "BodyHitBoxComponent.h"

BodyHitBoxComponent::BodyHitBoxComponent(Vector2Df position, float radius)
	: position(position), radius(radius)
{
	circle.setRadius(radius);
	circle.setFillColor(sf::Color(sf::Color::Transparent));
	circle.setOutlineColor(sf::Color(sf::Color::Green));
	circle.setOrigin(circle.getRadius(), circle.getRadius());
	circle.setOutlineThickness(3.f);
}
