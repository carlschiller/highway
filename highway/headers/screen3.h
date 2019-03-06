//
// Created by Carl Schiller on 2019-03-06.
//

#ifndef HIGHWAY_SCREEN3_H
#define HIGHWAY_SCREEN3_H

#include "cscreen.h"
#include "traffic.h"

class screen_3 : public cScreen{
private:
    bool run_bool;
    long sim_time;
    long frame_rate;
public:
    screen_3();
    virtual int Run(sf::RenderWindow & App, std::vector<float> * args,std::vector<bool> * bargs);
};

#endif //HIGHWAY_SCREEN3_H
