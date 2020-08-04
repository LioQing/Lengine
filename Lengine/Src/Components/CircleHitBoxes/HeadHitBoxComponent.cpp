#include "HeadHitBoxComponent.h"

HeadHitBoxComponent::HeadHitBoxComponent(Vector2Df position, float radius)
	:position (position), radius(radius)
{
	circle.setRadius(this->radius);
	circle.setFillColor(sf::Color(sf::Color::Transparent));
	circle.setOutlineColor(sf::Color(sf::Color::Green));
	circle.setOrigin(circle.getRadius(), circle.getRadius());
	circle.setOutlineThickness(3.f);
}
