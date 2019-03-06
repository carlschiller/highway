//
// Created by Carl Schiller on 2019-03-06.
//


#include <iostream>
#include "../headers/traffic.h"
#include "../headers/simulation2.h"
#include <cmath>
#include <unistd.h>

////////////////////////////////////////////////////////////////////////////////
/// Constructor
/// @param traffic : pointer reference to Traffic, this is to be able to
/// draw traffic outside of this class.
/// @param mutex : mutex thread lock from SFML.
/// @param sim_speed : Simulation speed multiplier, e.g. 10 would mean 10x
/// real time speed. If simulation can not keep up it lowers this.
/// @param framerate : Framerate of simulation, e.g. 60 FPS. This is the
/// time step of the system.
/// @param exit_bool : If user wants to exit this is changed outside of the class.

Sim::Sim(Traffic *&traffic, int framerate, int * time, bool * finish_bool):
        m_traffic(traffic),
        m_finish_bool(finish_bool),
        M_FRAMERATE(framerate),
        sim_time(time)
{

}

////////////////////////////////////////////////////////////////////////////////
/// Runs simulation. If M_SIM_SPEED = 10 , then it simulates 10x1/(M_FRAMERATE)
/// seconds of real time simulation.

void Sim::update() {
    sf::Clock clock;
    sf::Time time;
    double spawn_counter = 0.0;
    double threshold = 0.0;

    for(int i = 0; i < *sim_time*M_FRAMERATE; i++){
        m_traffic->update(1.0f/(float)M_FRAMERATE);
        m_traffic->spawn_cars(spawn_counter,1.0f/(float)M_FRAMERATE,threshold);
        m_traffic->despawn_cars();
    }

    *m_finish_bool = true;
}