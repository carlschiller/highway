//
// Created by Carl Schiller on 2019-03-04.
//

#ifndef HIGHWAY_CAR_H
#define HIGHWAY_CAR_H

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// Car                                                                  //
//                                                                      //
// Describes a car that moves around in Road class                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include <map>
#include "roadnode.h"
#include "roadsegment.h"

class Car{
private:
    float m_dist_to_next_node;
    float m_speed;
    float m_theta; // radians

    float m_aggressiveness; // how fast to accelerate;
    float m_target_speed;

public:
    Car();
    ~Car();
    Car(RoadSegment * spawn_point, int lane, float vel, float target_speed, float agressivness);
    Car(RoadSegment * spawn_point, RoadNode * lane, float vel, float target_speed, float agressivness);

    // all are raw pointers
    RoadSegment * current_segment;
    RoadNode * current_node;
    RoadNode * heading_to_node;
    Car * overtake_this_car;

    void update_pos(float delta_t);
    void merge(std::vector<RoadNode*> & connections);
    void do_we_want_to_overtake(Car * & closest_car, int & current_lane);
    void accelerate(float delta_t);
    void avoid_collision(float delta_t);
    Car * find_closest_car_ahead();
    std::map<Car *,bool> find_cars_around_car();

    float x_pos();
    float y_pos();

    float & speed();
    float & target_speed();
    float & theta();

    RoadSegment * get_segment();
};

#endif //HIGHWAY_CAR_H
