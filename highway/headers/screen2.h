//
// Created by Carl Schiller on 2019-03-05.
//

#ifndef HIGHWAY_SCREEN2_H
#define HIGHWAY_SCREEN2_H

#include "cscreen.h"

class screen_2 : public cScreen{
public:
    screen_2();
    virtual int Run(sf::RenderWindow & App, std::vector<float> * args,std::vector<bool> * bargs);
};


#endif //HIGHWAY_SCREEN2_H
