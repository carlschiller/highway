#include <iostream>
#include <vector>
#include "SFML/Graphics.hpp"
#include "../headers/simulation.h"
#include "../headers/unittests.h"
#include "../headers/screens.h"

int main() {
    std::vector<cScreen*> Screens;
    int screen = 0;

    sf::RenderWindow App(sf::VideoMode(550*2, 600*2), "Highway");
    App.setFramerateLimit(60);

    screen_0 s0;
    Screens.push_back(&s0);
    screen_1 s1;
    Screens.push_back(&s1);

    while(screen >= 0){
        screen = Screens[screen]->Run(App);
    }

    return 0;
}