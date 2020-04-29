#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

#include "Game.h"

int main()
{
    DeltaTime delta_time = 0u;
    sf::Clock delta_clock;

    sf::RenderWindow window(sf::VideoMode(800, 600), "Lengine");
    window.setFramerateLimit(60);

    Game game = Game(window);

    while (window.isOpen())
    {
        game.HandleEvent();
        game.Update(delta_time);
        if (delta_time != 0)std::cout << 1000/delta_time << std::endl;

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();
        }

        window.clear();
        game.Render();
        window.display();

        delta_time = delta_clock.restart().asMilliseconds();
    }

    return 0;
}