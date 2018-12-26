#include <iostream>
#include "SFML/Graphics.hpp"
#include "window.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(550*2, 600*2), "My window");
    window.setFramerateLimit(60);
    sf::Texture texture;
    if(!texture.loadFromFile("../mall2.png"))
    {

    }

    sf::Sprite background;
    background.setTexture(texture);
    background.scale(2.0f,2.0f);

    sf::Clock clock;
    sf::Clock t0;

    Car_squares cars = Car_squares();
    double spawn_counter = 0.0;
    double threshold = 0.0;

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed){
                window.close();
            }
        }

        sf::Time elapsed = clock.restart();

        cars.update(elapsed,spawn_counter,threshold);

        window.clear(sf::Color(255,255,255,255));

        window.draw(background);
        window.draw(cars);
        window.display();

        //cars.car_debug(t0.getElapsedTime());
    }

    return 0;
}