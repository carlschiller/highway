#include <iostream>
#include "SFML/Graphics.hpp"
#include "window.h"
#include "unittests.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(550*2, 600*2), "My window");
    window.setFramerateLimit(60);

    int sim_speed = 4;
    bool debug = true;
    bool super_debug = true;

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

    if(!super_debug){
        Simulation simulation = Simulation(debug,sim_speed);
        double spawn_counter = 0.0;
        double threshold = 0.0;

        sf::Text debug_info;

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

            simulation.update(elapsed,spawn_counter,threshold);

            window.clear(sf::Color(255,255,255,255));

            window.draw(background);
            window.draw(simulation);
            if(debug){
                simulation.get_info(debug_info,elapsed);
                window.draw(debug_info);
            }

            window.display();
        }
    }
    else{
        Tests tests = Tests();

        sf::Thread thread(&Tests::run_all_tests,&tests);

        thread.launch();

        sf::Text debug_info;
        // run the program as long as the window is open
        while (window.isOpen())
        {

            // check all the window's events that were triggered since the last iteration of the loop
            sf::Event event;
            while (window.pollEvent(event))
            {
                // "close requested" event: we close the window
                if (event.type == sf::Event::Closed){
                    thread.terminate();
                    window.close();
                }
            }

            sf::Time elapsed = clock.restart();

            window.clear(sf::Color(255,255,255,255));

            window.draw(background);
            window.draw(tests);

            tests.get_info(debug_info,elapsed);
            window.draw(debug_info);

            window.display();
        }
    }

    return 0;
}