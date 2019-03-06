//
// Created by Carl Schiller on 2019-03-04.
//

#ifndef HIGHWAY_CSCREEN_H
#define HIGHWAY_CSCREEN_H

#include "SFML/Graphics.hpp"
#include <vector>

class cScreen{
public:
    //virtual int Run(sf::RenderWindow & App) = 0;
    virtual int Run(sf::RenderWindow & App, std::vector<float> * args,std::vector<bool> * bargs) = 0;
};

#endif //HIGHWAY_CSCREEN_H
