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

void Rendering(sf::RenderWindow* window, int fps)
{
    float delay_time = static_cast<float>(1000) / fps;
    float delta_time = 0.f;
    sf::Clock delta_clock;
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

        delta_time = static_cast<float>(delta_clock.restart().asMicroseconds()) / 1000;
        if (delta_time < delay_time)
        {
            sf::sleep(sf::microseconds(fps - 1000 * delta_time));
        }
        delta_clock.restart();
    }
}

int main()
{
    DeltaTime delta_time = 0u;
    sf::Clock delta_clock;

    sf::RenderWindow window(sf::VideoMode(800, 640), "Lengine");
    int fps = 60;

    game = new Game(window);
    game->Init();

    window.setActive(false);
    std::thread render_thread(Rendering, &window, fps);

    while (window.isOpen())
    {
        std::unique_lock<std::mutex> l(m);
        cv.wait(l, [] { return !rendering; });

        game->HandleInput(delta_time);
        game->Update(delta_time);
        //if (delta_time != 0)std::cout << delta_time << std::endl;

        sf::sleep(sf::microseconds(1));

        delta_time = static_cast<float>(delta_clock.restart().asMicroseconds()) / 1000;
    }

    render_thread.join();

    return 0;
}