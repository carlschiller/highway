#include <iostream>
#include "SFML/Graphics.hpp"
#include "window.h"
#include "unittests.h"

sf::Mutex mutex;

int main() {
    sf::RenderWindow window(sf::VideoMode(550*2, 600*2), "My window");
    window.setFramerateLimit(60);

    int sim_speed = 1;
    bool debug = false;
    bool super_debug = false;

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

    bool exit_bool = false;

    if(!super_debug){
        sf::Mutex * mutex1 = &mutex;
        bool * exit = &exit_bool;
        //thread.launch();
        auto * traffic = new Traffic(debug);
        Simulation sim = Simulation(traffic, mutex1,sim_speed,60,exit);
        sf::Text debug_info;
        Traffic copy;

        sf::Thread thread(&Simulation::update,&sim);
        thread.launch();

        // run the program as long as the window is open
        while (window.isOpen())
        //while(false)
        {
            // check all the window's events that were triggered since the last iteration of the loop
            sf::Event event;
            while (window.pollEvent(event))
            {
                // "close requested" event: we close the window
                if (event.type == sf::Event::Closed){
                    exit_bool = true;
                    thread.wait();
                    window.close();
                }
            }
            sf::Time elapsed = clock.restart();

            mutex.lock();
            //std::cout << "copying\n";
            copy = *traffic;
            //std::cout << "copied\n";
            mutex.unlock();

            window.clear(sf::Color(255,255,255,255));

            window.draw(background);
            //mutex.lock();
            window.draw(copy);

            copy.get_info(debug_info,elapsed);

            //mutex.unlock();
            window.draw(debug_info);

            window.display();
        }

    }
    else{


        //sf::Thread thread(&Tests::run_all_tests,&tests);
        sf::Mutex * mutex1 = &mutex;
        //thread.launch();
        auto * traffic = new Traffic();
        Tests tests = Tests(traffic, mutex1);
        Traffic copy;
        sf::Text debug_info;

        sf::Thread thread(&Tests::run_all_tests,&tests);
        thread.launch();

        // run the program as long as the window is open
        while (window.isOpen())
        {

            // check all the window's events that were triggered since the last iteration of the loop
            sf::Event event;
            while (window.pollEvent(event))
            {
                // "close requested" event: we close the window
                if (event.type == sf::Event::Closed){
                    //thread.terminate();
                    window.close();
                    thread.terminate();
                    delete traffic;
                }
            }
            //Traffic copy = tests.m_traffic; // deep copy it
            sf::Time elapsed = clock.restart();

            window.clear(sf::Color(255,255,255,255));

            mutex.lock();
            copy = *traffic;
            mutex.unlock();

            window.draw(background);
            window.draw(copy);

            copy.get_info(debug_info,elapsed);
            window.draw(debug_info);

            window.display();
        }
    }

    return 0;
}