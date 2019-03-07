//
// Created by Carl Schiller on 2019-03-06.
//


#include <iostream>
#include "../headers/traffic.h"
#include "../headers/simulation2.h"
#include <cmath>
#include <unistd.h>
#include <iomanip>
#include <sstream>
#include <fstream>

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

Sim::Sim(Traffic *&traffic, int framerate, long * time, bool * finish_bool, int * percent):
        m_traffic(traffic),
        m_finish_bool(finish_bool),
        M_FRAMERATE(framerate),
        sim_time(time),
        m_percent(percent)
{

}

////////////////////////////////////////////////////////////////////////////////
/// Runs simulation. If M_SIM_SPEED = 10 , then it simulates 10x1/(M_FRAMERATE)
/// seconds of real time simulation.

void Sim::update() {
    sf::Clock clock;
    sf::Time time;
    double spawn_counter_0 = 0.0;
    double spawn_counter_1 = 0.0;
    double spawn_counter_2 = 0.0;
    double spawn_counter_3 = 0.0;

    long one_percent = *sim_time*M_FRAMERATE/100;
    int per = 0;

    std::vector<double *> counter;
    counter.push_back(&spawn_counter_0);
    counter.push_back(&spawn_counter_1);
    counter.push_back(&spawn_counter_2);
    counter.push_back(&spawn_counter_3);

    std::vector<double> answer;
    answer.reserve(*sim_time * M_FRAMERATE);

    for(int i = 0; i < *sim_time*M_FRAMERATE; i++){
        m_traffic->update(1.0f/(float)M_FRAMERATE);
        m_traffic->spawn_cars(counter,1.0f/(float)M_FRAMERATE);
        m_traffic->despawn_cars();
        answer.push_back(m_traffic->get_avg_flow());

        if(i%one_percent == 0){
            *m_percent = per;
            per++;
        }
    }

    print_to_file(&answer,*sim_time*M_FRAMERATE);

    *m_finish_bool = true;
}

void Sim::print_to_file(std::vector<double> * vec, long time_steps){
    std::string filename;
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%d-%m-%Y-%H-%M-%S");
    auto str = oss.str();

    filename += str + "steps" + std::to_string(time_steps) + ".txt";

    std::ofstream file_stream;
    file_stream.open(filename);

    for(auto subvec : *vec){
        file_stream << subvec << std::endl;
    }
    file_stream.close();

    std::cout << filename << " has been created\n";
}