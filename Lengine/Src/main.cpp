#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

#include "Game.h"

int main()
{
    Game game = Game();
    DeltaTime delta_time = 0u;

    sf::Clock delta_clock;
    sf::RenderWindow window(sf::VideoMode(800, 600), "Lengine");
    window.setFramerateLimit(60);

    while (window.isOpen())
    {
        game.Update(delta_time);
        std::cout << delta_time << std::endl;

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();
        }

        window.clear();

        window.display();

        delta_time = delta_clock.restart().asMilliseconds();
    }

    return 0;
}