#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

#include "Game.h"

Game* game;

int main()
{
    DeltaTime delta_time = 0u;
    sf::Clock delta_clock;

    sf::RenderWindow window(sf::VideoMode(800, 600), "Lengine");
    window.setFramerateLimit(60);

    game = new Game(window);
    game->Init();

    while (window.isOpen())
    {
        game->HandleInput(delta_time);
        game->Update(delta_time);
        if (delta_time != 0)std::cout << 1000/delta_time << std::endl;

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();
        }

        window.clear();
        game->Render();
        window.display();

        delta_time = static_cast<float>(delta_clock.restart().asMicroseconds()) / 1000;
    }

    return 0;
}