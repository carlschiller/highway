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

public:
    Traffic();
    explicit Traffic(bool debug);
    ~Traffic();
    Traffic(const Traffic&); // rule of three
    Traffic& operator=(const Traffic&); // rule of three

    unsigned long n_of_cars();
    void spawn_cars(double & spawn_counter, float elapsed, double & threshold);
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
