#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <atomic>

#include "Components/Components.h"
#include "Game.h"

std::atomic<Game*> game;

int main()
{
    DeltaTime delta_time = 0u;
    sf::Clock delta_clock;

    sf::RenderWindow window(sf::VideoMode(800, 640), "Lengine");
    window.setFramerateLimit(60);

    game = new Game(window);
    game.load()->Init();

    while (window.isOpen() || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
    {
        game.load()->HandleInput(delta_time);
        game.load()->Update(delta_time);
        // if (delta_time != 0) std::cout << 1000/delta_time << std::endl;

        window.clear();
        game.load()->Render();
        window.display();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            std::cout << "break pt. toggled" << std::endl;
        }

        delta_time = static_cast<float>(delta_clock.restart().asMicroseconds()) / 1000;
    }

    return 0;
}