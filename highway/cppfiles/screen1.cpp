//
// Created by Carl Schiller on 2019-03-04.
//

#include "../headers/screen1.h"
#include "../headers/traffic.h"
#include "../headers/simulation.h"
#include "../headers/unittests.h"
#include "button.h"
#include <iostream>

screen_1::screen_1() = default;

int screen_1::Run(sf::RenderWindow &App, std::vector<float> * args,std::vector<bool> * bargs) {
    sf::Mutex mutex;

    sf::Font font;

    sf::Texture texture;
    if(!texture.loadFromFile("../mall2.png"))
    {

    }

    if(!font.loadFromFile("/Library/Fonts/Andale mono.ttf")){
        return -1;
    }

    sf::Sprite background;
    background.setTexture(texture);
    //background.setColor(sf::Color::Black);
    background.scale(2.0f,2.0f);

    sf::Clock clock;
    sf::Clock t0;

    bool just_arrived = true;

    sf::Event event;

    bool exit_bool = false;

    sf::Time time1;

    sf::Mutex * mutex1 = &mutex;
    bool * exit = &exit_bool;
    //thread.launch();
    auto * traffic = new Traffic(*bargs,*args);
    Simulation sim = Simulation(traffic, mutex1,args[0][15],args[0][16],exit);
    sf::Text debug_info;

    sf::Thread thread(&Simulation::update,&sim);
    thread.launch();

    Button button = Button(font,24,0,215,"Go back",sf::Color(253,246,227),sf::Color::Black,sf::Color(253,235,227));
    button.center_text();

    while(true){
        // check all the window's events that were triggered since the last iteration of the loop

        while (App.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed){
                exit_bool = true;
                thread.wait();
                delete traffic;
                return -1;
            }

            if(event.type == sf::Event::MouseButtonPressed && just_arrived){

            }
            else if(!just_arrived){
                if(button.clicked(App)){
                    exit_bool = true;
                    thread.wait();
                    delete traffic;
                    return 0;
                }
            }
            else{
                just_arrived = false;
            }
        }

        sf::Time elapsed = clock.restart();

        mutex.lock();
        //std::cout << "copying\n";
        Traffic * copy = new Traffic(*traffic);
        //std::cout << "copied\n";
        mutex.unlock();

        App.clear(sf::Color(255,255,255,255));

        App.draw(background);
        //mutex.lock();
        App.draw(*copy);

        copy->get_info(debug_info,elapsed);

        //mutex.unlock();
        App.draw(debug_info);
        App.draw(button);

        App.display();
    }
    /*
    else{
        //sf::Thread thread(&Tests::run_all_tests,&tests);
        sf::Mutex * mutex1 = &mutex;
        //thread.launch();
        auto * traffic = new Traffic(debug,*args);
        Tests tests = Tests(traffic, mutex1);
        Traffic copy;
        sf::Text debug_info;

        sf::Thread thread(&Tests::run_all_tests,&tests);
        thread.launch();

        // run the program as long as the window is open
        while (true)
        {

            // check all the window's events that were triggered since the last iteration of the loop
            while (App.pollEvent(event))
            {
                // "close requested" event: we close the window
                if (event.type == sf::Event::Closed){
                    thread.terminate();
                    delete traffic;
                    return 0;
                }
            }
            //Traffic copy = tests.m_traffic; // deep copy it
            sf::Time elapsed = clock.restart();

            App.clear(sf::Color(255,255,255,255));

            mutex.lock();
            copy = *traffic;
            mutex.unlock();

            App.draw(background);
            App.draw(copy);

            copy.get_info(debug_info,elapsed);
            App.draw(debug_info);

            App.display();
        }
    }
    */
    return -1;
}