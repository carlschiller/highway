//
// Created by Carl Schiller on 2019-03-06.
//

#ifndef HIGHWAY_SIMULATION2_H
#define HIGHWAY_SIMULATION2_H

#include <vector>
#include "SFML/Graphics.hpp"
#include "traffic.h"

class Sim{
private:
    Traffic * m_traffic;
    bool * m_finish_bool;
    const int M_FRAMERATE;
    int * sim_time;
public:
    Sim() = delete;
    Sim(Traffic *& traffic, int m_framerate, int * time, bool * exitbool);

    void update();
};



#endif //HIGHWAY_SIMULATION2_H
