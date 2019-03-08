//
// Created by Carl Schiller on 2019-03-04.
//

#include <iostream>
#include "../headers/traffic.h"
#include "../headers/simulation.h"
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

Simulation::Simulation(Traffic *&traffic, sf::Mutex *&mutex, int sim_speed, int framerate, bool *& exit_bool):
        m_mutex(mutex),
        m_traffic(traffic),
        m_exit_bool(exit_bool),
        M_SIM_SPEED(sim_speed),
        M_FRAMERATE(framerate)
{

}

////////////////////////////////////////////////////////////////////////////////
/// Runs simulation. If M_SIM_SPEED = 10 , then it simulates 10x1/(M_FRAMERATE)
/// seconds of real time simulation.

void Simulation::update() {
    sf::Clock clock;
    sf::Time time;
    double spawn_counter_0 = 0.0;
    double spawn_counter_1 = 0.0;
    double spawn_counter_2 = 0.0;
    double spawn_counter_3 = 0.0;

    std::vector<double *> counter;
    counter.push_back(&spawn_counter_0);
    counter.push_back(&spawn_counter_1);
    counter.push_back(&spawn_counter_2);
    counter.push_back(&spawn_counter_3);

    while(!*m_exit_bool){
        m_mutex->lock();
        //std::cout << "calculating\n";
        for(int i = 0; i < M_SIM_SPEED; i++){
            //std::cout<< "a\n";
            m_traffic->update(1.0f/(float)M_FRAMERATE);
            //std::cout<< "b\n";
            m_traffic->spawn_cars(counter,1.0f/(float)M_FRAMERATE);
            //m_mutex->lock();
            //std::cout<< "c\n";
            m_traffic->despawn_cars();
            //m_mutex->unlock();
            //std::cout<< "d\n";
        }
        //std::cout << "calculated\n";
        m_mutex->unlock();

        time = clock.restart();
        sf::Int64 acutal_elapsed = time.asMicroseconds();
        double sim_elapsed = (1.0f/(float)M_FRAMERATE)*1000000;

        if(acutal_elapsed < sim_elapsed){
            usleep((useconds_t)(sim_elapsed-acutal_elapsed));
            m_traffic->m_multiplier = M_SIM_SPEED;
        }
        else{
            m_traffic->m_multiplier = M_SIM_SPEED*(sim_elapsed/acutal_elapsed);
        }
    }
}
