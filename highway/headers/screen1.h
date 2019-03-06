//
// Created by Carl Schiller on 2019-03-04.
//

#ifndef HIGHWAY_SCREEN1_H
#define HIGHWAY_SCREEN1_H

#include "cscreen.h"

class screen_1 : public cScreen{
public:
    screen_1();
    virtual int Run(sf::RenderWindow & App, std::vector<float> * args, std::vector<bool> * bargs);
};

#endif //HIGHWAY_SCREEN1_H
