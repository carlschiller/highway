//
// Created by Carl Schiller on 2018-12-19.
//

#include <vector>
#include "SFML/Graphics.hpp"
#include "traffic.h"

#ifndef HIGHWAY_WINDOW_H
#define HIGHWAY_WINDOW_H

class Simulation{
private:
    sf::Mutex * m_mutex;
    Traffic * m_traffic;
    bool * m_exit_bool;
    const int M_SIM_SPEED;
    const int M_FRAMERATE;
public:
    Simulation() = delete;
    Simulation(Traffic *& traffic,sf::Mutex *& mutex, int sim_speed, int m_framerate, bool *& exitbool);

    void update();
};


#endif //HIGHWAY_WINDOW_H
