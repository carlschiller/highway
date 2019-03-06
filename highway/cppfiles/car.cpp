//
// Created by Carl Schiller on 2019-03-04.
//

#include "../headers/car.h"
#include <map>
#include <cmath>
#include <list>
#include <iostream>
#include "../headers/util.h"

////////////////////////////////////////////////////////////////////////////////
/// Constructor.

Car::Car() :
        m_speed(0),
        m_aggressiveness(0),
        m_target_speed(0),
        m_min_dist_to_car_in_front(0),
        m_min_overtake_dist_trigger(0),
        m_max_overtake_dist_trigger(0),
        m_overtake_done_dist(0),
        m_merge_min_dist(0),
        m_search_radius_around(0),
        m_search_radius_to_car_in_front(0),
        current_segment(nullptr),
        current_node(nullptr),
        overtake_this_car(nullptr)
{

}


////////////////////////////////////////////////////////////////////////////////
/// Constructor for new car with specified lane numbering in spawn point.
/// Lane numbering @param lane must not exceed amount of lanes in
/// @param spawn_point, otherwise an exception will be thrown.

Car::Car(RoadSegment * spawn_point, int lane, float vel, float target_speed, float agressivness,
         float m_min_dist_to_car_in_front, float m_min_overtake_dist_trigger, float m_max_overtake_dist_trigger,
         float m_overtake_done_dist, float m_merge_min_dist, float m_search_radius_around,
         float m_search_radius_to_car_in_front) :
        m_speed(vel),
        m_aggressiveness(agressivness),
        m_target_speed(target_speed),
        m_min_dist_to_car_in_front(m_min_dist_to_car_in_front),
        m_min_overtake_dist_trigger(m_min_overtake_dist_trigger),
        m_max_overtake_dist_trigger(m_max_overtake_dist_trigger),
        m_overtake_done_dist(m_overtake_done_dist),
        m_merge_min_dist(m_merge_min_dist),
        m_search_radius_around(m_search_radius_around),
        m_search_radius_to_car_in_front(m_search_radius_to_car_in_front),
        current_segment(spawn_point),
        current_node(current_segment->get_node_pointer(lane)),
        overtake_this_car(nullptr)
{
    current_segment->append_car(this);

    if(!current_node->get_nodes_from_me().empty()){
        heading_to_node = current_node->get_next_node(lane);

        m_dist_to_next_node = Util::distance(current_node->get_x(),heading_to_node->get_x(),current_node->get_y(),heading_to_node->get_y());

        m_theta = current_node->get_theta(heading_to_node);
    }
    else{
        throw std::invalid_argument("Car spawns in node with empty connections, or with a nullptr segment");
    }
}

////////////////////////////////////////////////////////////////////////////////
/// Constructor for new car with specified lane. Note that
/// @param lane must be in @param spawn_point, otherwise no guarantee on
/// functionality.

Car::Car(RoadSegment * spawn_point, RoadNode * lane, float vel, float target_speed, float agressivness,
         float m_min_dist_to_car_in_front, float m_min_overtake_dist_trigger, float m_max_overtake_dist_trigger,
         float m_overtake_done_dist, float m_merge_min_dist, float m_search_radius_around,
         float m_search_radius_to_car_in_front):
        m_speed(vel),
        m_aggressiveness(agressivness),
        m_target_speed(target_speed),
        m_min_dist_to_car_in_front(m_min_dist_to_car_in_front),
        m_min_overtake_dist_trigger(m_min_overtake_dist_trigger),
        m_max_overtake_dist_trigger(m_max_overtake_dist_trigger),
        m_overtake_done_dist(m_overtake_done_dist),
        m_merge_min_dist(m_merge_min_dist),
        m_search_radius_around(m_search_radius_around),
        m_search_radius_to_car_in_front(m_search_radius_to_car_in_front),
        current_segment(spawn_point),
        current_node(lane),
        overtake_this_car(nullptr)
{
    current_segment->append_car(this);

    if(!current_node->get_nodes_from_me().empty() || current_segment->next_segment() != nullptr){
        heading_to_node = current_node->get_next_node(0);

        m_dist_to_next_node = Util::distance(current_node->get_x(),heading_to_node->get_x(),current_node->get_y(),heading_to_node->get_y());

        m_theta = current_node->get_theta(heading_to_node);
    }
    else{
        throw std::invalid_argument("Car spawns in node with empty connections, or with a nullptr segment");
    }
}

////////////////////////////////////////////////////////////////////////////////
/// Destructor for car.

Car::~Car(){
    if(this->current_segment != nullptr){
        this->current_segment->remove_car(this); // remove this pointer shit
    }

    overtake_this_car = nullptr;
    current_segment = nullptr;
    heading_to_node = nullptr;
    current_node = nullptr;
}

////////////////////////////////////////////////////////////////////////////////
/// Updates position for car with time step @param delta_t.

void Car::update_pos(float delta_t) {
    m_dist_to_next_node -= m_speed*delta_t;
    // if we are at a new node.

    if(m_dist_to_next_node < 0){
        current_segment->remove_car(this); // remove car from this segment
        current_segment = heading_to_node->get_parent_segment(); // set new segment
        if(current_segment != nullptr){
            current_segment->append_car(this); // add car to new segment
            if(current_segment->meter){
                current_segment->car_passed = true;
            }
        }

        current_node = heading_to_node; // set new current node as previous one.

        //TODO: place logic for choosing next node
        std::vector<RoadNode*> connections = current_node->get_nodes_from_me();

        if(!connections.empty()){

            merge(connections);

            m_dist_to_next_node += Util::distance(current_node->get_x(),heading_to_node->get_x(),current_node->get_y(),heading_to_node->get_y());
            m_theta = current_node->get_theta(heading_to_node);

        }
    }
}

////////////////////////////////////////////////////////////////////////////////
/// Function to determine if we can merge into another lane depending on.
/// properties of @param connections.

void Car::merge(std::vector<RoadNode*> & connections) {
    // check if we merge
    int current_lane = current_segment->get_lane_number(current_node);
    bool can_merge = true;
    std::map<Car*,bool> cars_around_car = find_cars_around_car();
    Car * closest_car = find_closest_car_ahead();

    for(auto it : cars_around_car){
        float delta_dist = Util::distance_to_car(it.first,this);
        float delta_speed = abs(speed()-it.first->speed());

        if(current_lane == 0 && it.first->heading_to_node->get_parent_segment()->get_lane_number(it.first->heading_to_node) == 1 ){
            can_merge =
                    delta_dist > std::max(delta_speed*4.0f/m_aggressiveness,m_merge_min_dist);
        }
        else if(current_lane == 1 && it.first->heading_to_node->get_parent_segment()->get_lane_number(it.first->heading_to_node) == 0){
            can_merge =
                    delta_dist > std::max(delta_speed*4.0f/m_aggressiveness,m_merge_min_dist);
        }

        if(!can_merge){
            break;
        }
    }

    if(current_segment->merge){
        if(current_lane == 0 && connections[0]->get_parent_segment()->get_total_amount_of_lanes() != 2){
            if(can_merge){
                heading_to_node = connections[1];
            }
            else{
                heading_to_node = connections[0];
            }
        }
        else if(connections[0]->get_parent_segment()->get_total_amount_of_lanes() == 2){
            current_lane = std::max(current_lane-1,0);
            heading_to_node = connections[current_lane];
        }
        else{
            heading_to_node = connections[current_lane];
        }
    }
        // if we are in start section
    else if(current_segment->get_total_amount_of_lanes() == 3){
        if(connections.size() == 1){
            heading_to_node = connections[0];
        }
        else{
            heading_to_node = connections[current_lane];
        }
    }
        // if we are in middle section
    else if(current_segment->get_total_amount_of_lanes() == 2){
        // normal way
        if(connections[0]->get_parent_segment()->get_total_amount_of_lanes() == 2){
            // check if we want to overtake car in front
            do_we_want_to_overtake(closest_car,current_lane);

            // commited to overtaking
            if(overtake_this_car != nullptr){
                if(current_lane != 1){
                    if(can_merge){
                        heading_to_node = connections[1];
                    }
                    else{
                        heading_to_node = connections[current_lane];
                    }
                }
                else{
                    heading_to_node = connections[current_lane];
                }

            }
                // merge back if overtake this car is nullptr.
            else{
                if(can_merge){
                    heading_to_node = connections[0];
                }
                else{
                    heading_to_node = connections[current_lane];
                }
            }

        }
        else{
            heading_to_node = connections[0];
        }
    }
    else if(current_segment->get_total_amount_of_lanes() == 1){
        heading_to_node = connections[0];
    }
}

////////////////////////////////////////////////////////////////////////////////
/// Helper function to determine if this car wants to overtake
/// @param closest_car.

void Car::do_we_want_to_overtake(Car * & closest_car, int & current_lane) {
    //see if we want to overtake car.

    if(closest_car != nullptr){
        //float delta_speed = closest_car->speed()-speed();
        float delta_distance = Util::distance_to_car(this,closest_car);

        if(overtake_this_car == nullptr){
            if(delta_distance > m_min_overtake_dist_trigger && delta_distance < m_max_overtake_dist_trigger && (target_speed()/closest_car->target_speed() > m_aggressiveness*1.0f ) && current_lane == 0 && closest_car->current_node->get_parent_segment()->get_lane_number(closest_car->current_node) == 0){
                overtake_this_car = closest_car;
            }
        }

    }

    if(overtake_this_car !=nullptr){
        if(Util::is_car_behind(overtake_this_car,this) && (Util::distance_to_car(this,overtake_this_car) > m_overtake_done_dist)){
            overtake_this_car = nullptr;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
/// Function to accelerate this car.

void Car::accelerate(float elapsed){
    float target = m_target_speed;
    float d_vel; // proportional control.

    if(m_speed < target*0.75){
        d_vel = m_aggressiveness*elapsed*2.0f;
    }
    else{
        d_vel = m_aggressiveness*(target-m_speed)*4*elapsed*2.0f;
    }

    m_speed += d_vel;
}

////////////////////////////////////////////////////////////////////////////////
/// Helper function to avoid collision with another car.

void Car::avoid_collision(float delta_t) {
    float min_distance = m_min_dist_to_car_in_front; // for car distance.
    float ideal = min_distance+min_distance*(m_speed/20.f);

    Car * closest_car = find_closest_car_ahead();
    float detection_distance = m_speed*5.0f;

    if(closest_car != nullptr) {
        float radius_to_car = Util::distance_to_car(this, closest_car);
        float delta_speed = closest_car->speed() - this->speed();

        if (radius_to_car < ideal && delta_speed < 0 && radius_to_car > min_distance) {
            m_speed -= std::max(std::max((radius_to_car-min_distance)*0.5f,0.0f),10.0f*delta_t);
        }
        else if(radius_to_car < min_distance){
            m_speed -= std::max(std::max((min_distance-radius_to_car)*0.5f,0.0f),2.0f*delta_t);
        }
        else if(delta_speed < 0 && radius_to_car < detection_distance){
            m_speed -= std::min(
                    abs(pow(delta_speed, 2.0f)) * pow(ideal * 0.25f / radius_to_car, 2.0f) * m_aggressiveness * 0.15f,
                    10.0f * delta_t);
        }
        else {
            accelerate(delta_t);
        }

        if(current_segment->merge){
            std::map<Car*,bool> around = find_cars_around_car();
            for(auto it : around){
                float delta_dist = Util::distance_to_car(it.first,this);
                delta_speed = abs(speed()-it.first->speed());

                if(it.first->current_node->get_parent_segment()->get_lane_number(it.first->current_node) == 0 && delta_dist < ideal && this->current_segment->get_lane_number(current_node) == 1 && speed()/target_speed() > 0.5){
                    if(Util::is_car_behind(it.first,this)){
                        accelerate(delta_t);
                    }
                    else{
                        m_speed -= std::max(std::max((ideal-delta_dist)*0.5f,0.0f),10.0f*delta_t);
                    }
                }
                else if(it.first->current_node->get_parent_segment()->get_lane_number(it.first->current_node) == 1 && this->current_segment->get_lane_number(current_node) == 0 && speed()/target_speed() > 0.5 && delta_dist < ideal){
                    if(Util::is_car_behind(this,it.first)){
                        m_speed -= std::max(std::max((ideal-delta_dist)*0.5f,0.0f),10.0f*delta_t);
                    }
                    else{
                        accelerate(delta_t);
                    }
                }
            }
        }
        else{

        }
    }

    if(heading_to_node->get_parent_segment()->meter){
        if(heading_to_node->get_parent_segment()->car_passed || heading_to_node->get_parent_segment()->ramp_counter < heading_to_node->get_parent_segment()->period*0.5f){
            if (m_dist_to_next_node < ideal) {
                m_speed -= std::max(std::max((m_dist_to_next_node-min_distance)*0.5f,0.0f),10.0f*delta_t);
            }
            else if(m_dist_to_next_node < detection_distance){
                m_speed -= std::min(
                        abs(pow(m_speed, 2.0f)) * pow(ideal * 0.25f / m_dist_to_next_node, 2.0f) * m_aggressiveness * 0.15f,
                        10.0f * delta_t);
            }
        }
        else{
            accelerate(delta_t);
        }
    }
    else{
        accelerate(delta_t);
    }

    if(m_speed < 0){
        m_speed = 0;
    }


}

////////////////////////////////////////////////////////////////////////////////
/// Helper function to find closest car in the same lane ahead of this car.
/// Returns a car if found, otherwise nullptr.

Car* Car::find_closest_car_ahead() {
    float search_radius = m_search_radius_to_car_in_front;
    std::map<RoadNode*,bool> visited;
    std::list<RoadNode*> queue;

    for(RoadNode * node : (this->current_segment->get_nodes())){
        queue.push_front(node);
    }

    Car* answer = nullptr;

    float shortest_distance = 10000000;

    while(!queue.empty()){
        RoadNode * next_node = queue.back(); // get last element
        queue.pop_back(); // remove element

        if(next_node != nullptr){
            if(!visited[next_node] && Util::distance(x_pos(),next_node->get_x(),y_pos(),next_node->get_y()) < search_radius){
                visited[next_node] = true;

                for(Car * car : next_node->get_parent_segment()->m_cars){
                    if(this != car){
                        float radius = Util::distance_to_car(this,car);
                        if(Util::is_car_behind(this,car) && Util::will_car_paths_cross(this,car) && radius < shortest_distance){
                            shortest_distance = radius;
                            answer = car;
                        }

                    }
                }

                // push in new nodes in front of list.
                for(RoadNode * node : next_node->get_nodes_from_me()){
                    queue.push_front(node);
                }
            }
        }
    }
    return answer;
}

////////////////////////////////////////////////////////////////////////////////
/// Searches for cars around this car in a specified radius. Note that
/// search radius is the radius to RoadNodes, and not surrounding cars.
/// Returns a map of cars the function has found.

std::map<Car *,bool> Car::find_cars_around_car() {
    float search_radius = m_search_radius_around;
    std::map<RoadNode*,bool> visited;
    std::list<RoadNode*> queue;

    for(RoadNode * node : (this->current_segment->get_nodes())){
        queue.push_front(node);
    }

    std::map<Car *,bool> answer;
    while(!queue.empty()){
        RoadNode * next_node = queue.back(); // get last element
        queue.pop_back(); // remove element

        if(next_node != nullptr){
            if(!visited[next_node] && Util::distance(x_pos(),next_node->get_x(),y_pos(),next_node->get_y()) < search_radius){
                visited[next_node] = true;
                for(Car * car : next_node->get_parent_segment()->m_cars){
                    if(this != car){
                        answer[car] = true;
                    }
                }
                // push in new nodes in front of list.
                for(RoadNode * node : next_node->get_nodes_from_me()){
                    queue.push_front(node);
                }

                for(RoadNode * node: next_node->get_nodes_to_me()){
                    queue.push_front(node);
                }
            }
        }
    }
    return answer;
}

////////////////////////////////////////////////////////////////////////////////
/// Returns x position of car.

float Car::x_pos() {
    float x_position;
    if(heading_to_node != nullptr){
        x_position = heading_to_node->get_x()-m_dist_to_next_node*cos(m_theta);
    }
    else{
        x_position = current_node->get_x();
    }

    return x_position;
}

////////////////////////////////////////////////////////////////////////////////
/// Returns y position of car.

float Car::y_pos() {
    float y_position;
    if(heading_to_node != nullptr){
        y_position = heading_to_node->get_y()+m_dist_to_next_node*sin(m_theta);
    }
    else{
        y_position = current_node->get_y();
    }

    return y_position;
}

////////////////////////////////////////////////////////////////////////////////
/// Returns speed of car, as reference.

float & Car::speed() {
    return m_speed;
}

////////////////////////////////////////////////////////////////////////////////
/// Returns target speed of car as reference.

float & Car::target_speed() {
    return m_target_speed;
}

////////////////////////////////////////////////////////////////////////////////
/// Returns theta of car, the direction of the car. Defined in radians as a
/// mathematitan would define angles.

float & Car::theta() {
    return m_theta;
}

////////////////////////////////////////////////////////////////////////////////
/// Returns current segment car is in.

RoadSegment* Car::get_segment() {
    return current_segment;
}