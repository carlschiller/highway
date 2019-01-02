#include <iostream>
#include "SFML/Graphics.hpp"
#include "window.h"

int main() {
    if(false){
        Road road = Road();
    }
    else{
        sf::RenderWindow window(sf::VideoMode(550*2, 600*2), "My window");
        window.setFramerateLimit(60);

        float sim_speed = 1.0f;

        std::string speedy = " fps, speed: " + std::to_string(sim_speed).substr(0,1) + " x";

        sf::Font font;
        if (!font.loadFromFile("/Library/Fonts/Arial.ttf"))
        {
            // error...
        }

        sf::Text fps;
        fps.setFont(font);
        fps.setCharacterSize(24);
        fps.setFillColor(sf::Color::Red);
        fps.setStyle(sf::Text::Bold);

        sf::Texture texture;
        if(!texture.loadFromFile("../mall2.png"))
        {

        }

        sf::Sprite background;
        background.setTexture(texture);
        //background.setColor(sf::Color::Black);
        background.scale(2.0f,2.0f);

        sf::Clock clock;
        sf::Clock t0;

        Car_squares cars = Car_squares();
        double spawn_counter = 0.0;
        double threshold = 0.0;

        std::cout << "starting\n";

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

            float frames = 1/elapsed.asSeconds();

            cars.update(elapsed,spawn_counter,threshold,sim_speed);
            float flow = cars.get_flow();
            window.clear(sf::Color(255,255,255,255));

            std::string frm = std::to_string(frames).substr(0,2) + speedy + "\nFlow: " + std::to_string(flow).substr(0,4);
            fps.setString(frm);

            window.draw(background);
            window.draw(fps);
            window.draw(cars);
            window.display();

            //cars.car_debug(t0.getElapsedTime());
        }
    }


    return 0;
}