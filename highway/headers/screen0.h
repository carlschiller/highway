//
// Created by Carl Schiller on 2019-03-04.
//

#ifndef HIGHWAY_SCREEN0_H
#define HIGHWAY_SCREEN0_H

#include "cscreen.h"

class screen_0 : public cScreen{
public:
    screen_0();
    virtual int Run(sf::RenderWindow & App, std::vector<float> * args);
};

#endif //HIGHWAY_SCREEN0_H
