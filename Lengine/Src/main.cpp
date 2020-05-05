#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "Components/Components.h"
#include "Game.h"

Game* game;

std::mutex m;
std::condition_variable cv;
bool rendering = false;

void Rendering(sf::RenderWindow* window)
{
    window->setActive(true);

    while (window->isOpen())
    {
        window->clear();

        rendering = true;
        std::unique_lock<std::mutex> l(m);

        game->Render();

        rendering = false;
        cv.notify_one();

        window->display();
    }
}

int main()
{
    DeltaTime delta_time = 0u;
    sf::Clock delta_clock;

    sf::RenderWindow window(sf::VideoMode(800, 640), "Lengine");
    window.setFramerateLimit(60);

    game = new Game(window);
    game->Init();

    window.setActive(false);
    std::thread render_thread(Rendering, &window);

    while (window.isOpen())
    {
        std::unique_lock<std::mutex> l(m);
        cv.wait(l, [] { return !rendering; });

        game->HandleInput(delta_time);
        game->Update(delta_time);
        //if (delta_time != 0)std::cout << delta_time << std::endl;

        delta_time = static_cast<float>(delta_clock.restart().asMicroseconds()) / 1000;
    }

    render_thread.join();

    return 0;
}