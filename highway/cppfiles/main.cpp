#include <iostream>
#include <vector>
#include "SFML/Graphics.hpp"
#include "../headers/simulation.h"
#include "../headers/unittests.h"
#include "../headers/screens.h"

int main() {
    std::vector<cScreen*> Screens;
    int screen = 0;

    sf::RenderWindow App(sf::VideoMode(550*2, 600*2), "Highway");
    App.setFramerateLimit(60);

    screen_0 s0;
    Screens.push_back(&s0);
    screen_1 s1;
    Screens.push_back(&s1);
    screen_2 s2;
    Screens.push_back(&s2);

    std::vector<float> args;

    float m_aggro = 1.0f;
    args.push_back(m_aggro);
    float m_aggro_sigma = 0.2f;
    args.push_back(m_aggro_sigma);
    float m_spawn_freq = 1.0f;
    args.push_back(m_spawn_freq);
    float m_speed = 20.f;
    args.push_back(m_speed);

    float m_lane_0_spawn_prob = 0.9f/3;
    args.push_back(m_lane_0_spawn_prob);
    float m_lane_1_spawn_prob = 0.9f/3;
    args.push_back(m_lane_1_spawn_prob);
    float m_lane_2_spawn_prob = 0.9f/3;
    args.push_back(m_lane_2_spawn_prob);
    float m_ramp_0_spawn_prob = 0.1f/2;
    args.push_back(m_ramp_0_spawn_prob);

    float m_min_dist_to_car_in_front = 8;
    args.push_back(m_min_dist_to_car_in_front);
    float m_min_overtake_dist_trigger = 10;
    args.push_back(m_min_overtake_dist_trigger);
    float m_max_overtake_dist_trigger = 40;
    args.push_back(m_max_overtake_dist_trigger);
    float m_overtake_done_dist = 30;
    args.push_back(m_overtake_done_dist);
    float m_merge_min_dist = 15.0f;
    args.push_back(m_merge_min_dist);
    float m_search_radius_around = 30;
    args.push_back(m_search_radius_around);
    float m_search_radius_to_car_in_front = 50;
    args.push_back(m_search_radius_to_car_in_front);
    float sim_speed = 10;
    args.push_back(sim_speed);
    float framerate = 60;
    args.push_back(framerate);

    std::vector<bool> bool_args;
    bool debug = false;
    bool_args.push_back(debug);
    bool ramp_meter = false;
    bool_args.push_back(ramp_meter);

    while(screen >= 0){
        screen = Screens[screen]->Run(App,&args,&bool_args);
    }

    return 0;
}