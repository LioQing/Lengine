#include "CameraManager.h"

void CameraManager::Draw(sf::RenderWindow* window)
{
	camera.setCenter(follow->sfVector2f());
    window->setView(camera);
}

void CameraManager::SetCenter(Vector2Df center)
{
	camera.setCenter(center.sfVector2f());
}

void CameraManager::SetWindow(sf::RenderWindow* window)
{
	camera.setSize(sf::Vector2f(window->getSize()));
}

void CameraManager::SetFollow(Vector2Df* follow)
{
	this->follow = follow;
}

sf::FloatRect CameraManager::GetViewBounds()
{
    sf::FloatRect rt;
    rt.left = camera.getCenter().x - camera.getSize().x / 2.f;
    rt.top = camera.getCenter().y - camera.getSize().y / 2.f;
    rt.width = camera.getSize().x;
    rt.height = camera.getSize().y;
    return rt;
}