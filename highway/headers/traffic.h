//
// Created by Carl Schiller on 2018-12-19.
//

#ifndef HIGHWAY_TRAFFIC_H
#define HIGHWAY_TRAFFIC_H

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// Traffic                                                              //
//                                                                      //
// Describes the whole traffic situation with Cars and a Road.          //
// Inherits form SFML Graphics.hpp in order to render the cars.         //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include <random>
#include <vector>
#include "SFML/Graphics.hpp"
#include "car.h"

class Traffic : public sf::Drawable, public sf::Transformable{
private:
    std::vector<Car*> m_cars;
    bool debug;
    std::mt19937 & my_engine();
    sf::Font m_font;

    const float m_aggro;
    const float m_aggro_sigma;
    const float m_spawn_freq;
    const float m_speed;

    const float m_lane_0_spawn_prob;
    const float m_lane_1_spawn_prob;
    const float m_lane_2_spawn_prob;
    const float m_ramp_0_spawn_prob;

    const float m_min_dist_to_car_in_front;
    const float m_min_overtake_dist_trigger;
    const float m_max_overtake_dist_trigger;
    const float m_overtake_done_dist;
    const float m_merge_min_dist;
    const float m_search_radius_around;
    const float m_search_radius_to_car_in_front;

    const float m_ramp_meter_period;
    const bool m_ramp_meter;

    float road_length;

    std::vector<float> probs;
public:
    Traffic() = delete;
    Traffic(std::vector<bool> bargs, std::vector<float> args);
    ~Traffic();
    Traffic(const Traffic&); // rule of three
    Traffic& operator=(const Traffic&); // rule of three

    unsigned long n_of_cars();
    void spawn_cars(std::vector<double*> & counters, float elapsed);
    void despawn_cars();
    void despawn_all_cars();
    void despawn_car(Car*& car);
    void force_place_car(RoadSegment * seg, RoadNode * node, float vel, float target, float aggro);


    void update(float elapsed_time);
    std::vector<Car *> get_car_copies() const;
    float get_avg_flow();
    std::vector<float> get_avg_speeds();
private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
public:
    void get_info(sf::Text & text, sf::Time &elapsed);
    double m_multiplier;
};

#endif //HIGHWAY_TRAFFIC_H
