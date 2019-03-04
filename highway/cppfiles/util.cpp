//
// Created by Carl Schiller on 2019-03-04.
//

#include "../headers/util.h"
#include <sstream>
#include <string>
#include <cmath>

////////////////////////////////////////////////////////////////////////////////
/// Splits @param str by @param delim, returns vector of tokens obtained.

std::vector<std::string> Util::split_string_by_delimiter(const std::string &str, const char delim) {
    std::stringstream ss(str);
    std::string item;
    std::vector<std::string> answer;
    while(std::getline(ss,item,delim)){
        answer.push_back(item);
    }
    return answer;
}

////////////////////////////////////////////////////////////////////////////////
/// Returns true if @param a is behind @param b, else false

bool Util::is_car_behind(Car * a, Car * b){
    if(a!=b){
        float theta_to_car_b = atan2(a->y_pos()-b->y_pos(),b->x_pos()-a->x_pos());
        float theta_difference = get_min_angle(a->theta(),theta_to_car_b);
        return theta_difference < M_PI*0.45;
    }
    else{
        return false;
    }

}

////////////////////////////////////////////////////////////////////////////////
/// Returns true if @param a will cross paths with @param b, else false.
/// NOTE: @param a MUST be behind @param b.

bool Util::will_car_paths_cross(Car *a, Car *b) {
    //simulate car a driving straight ahead.
    RoadSegment * inspecting_segment = a->get_segment();
    //RoadNode * node_0 = a->current_node;
    RoadNode * node_1 = a->heading_to_node;

    //int node_0_int = inspecting_segment->get_lane_number(node_0);
    int node_1_int = node_1->get_parent_segment()->get_lane_number(node_1);

    while(!node_1->get_nodes_from_me().empty()){
        for(Car * car : inspecting_segment->m_cars){
            if(car == b){
                // place logic for evaluating if we cross cars here.
                // heading to same node, else return false
                return node_1 == b->heading_to_node;
            }
        }

        inspecting_segment = node_1->get_parent_segment();
        //node_0_int = node_1_int;
        //node_0 = node_1;

        // if we are at say, 2 lanes and heading to 2 lanes, keep previous lane numbering.
        if(inspecting_segment->get_total_amount_of_lanes() == node_1->get_nodes_from_me().size()){
            node_1 = node_1->get_nodes_from_me()[node_1_int];
        }
            // if we get one option, stick to it.
        else if(node_1->get_nodes_from_me().size() == 1){
            node_1 = node_1->get_nodes_from_me()[0];

        }
            // we merge from 3 to 2.
        else if(inspecting_segment->get_total_amount_of_lanes() == 3 && inspecting_segment->merge){
            node_1 = node_1->get_nodes_from_me()[std::max(node_1_int-1,0)];
        }

        node_1_int = node_1->get_parent_segment()->get_lane_number(node_1);
    }

    return false;
}

/*

bool Util::merge_helper(Car *a, int merge_to_lane) {
    RoadSegment * seg = a->current_segment;
    for(Car * car : seg->m_cars){
        if(car != a){
            float delta_speed = a->speed()-car->speed();
            if(car->heading_to_node == a->current_node->get_nodes_from_me()[merge_to_lane] && delta_speed < 0){
                return true;
            }
        }
    }
    return false;
}

*/

/*

// this works only if a's heading to is b's current segment
bool Util::is_cars_in_same_lane(Car *a, Car *b) {
    return a->heading_to_node == b->current_node;
}

*/

/*
float Util::distance_to_line(const float theta, const float x, const float y){
    float x_hat,y_hat;
    x_hat = cos(theta);
    y_hat = -sin(theta);

    float proj_x = (x*x_hat+y*y_hat)*x_hat;
    float proj_y = (x*x_hat+y*y_hat)*y_hat;
    float dist = sqrt(abs(pow(x-proj_x,2.0f))+abs(pow(y-proj_y,2.0f)));

    return dist;
}
*/

/*
float Util::distance_to_proj_point(const float theta, const float x, const float y){
    float x_hat,y_hat;
    x_hat = cos(theta);
    y_hat = -sin(theta);
    float proj_x = (x*x_hat+y*y_hat)*x_hat;
    float proj_y = (x*x_hat+y*y_hat)*y_hat;
    float dist = sqrt(abs(pow(proj_x,2.0f))+abs(pow(proj_y,2.0f)));

    return dist;
}
*/

////////////////////////////////////////////////////////////////////////////////
/// Returns distance between @param a and @param b.

float Util::distance_to_car(Car * a, Car * b){
    if(a == nullptr || b == nullptr){
        throw std::invalid_argument("Can't calculate distance if cars are nullptrs");
    }

    float delta_x = a->x_pos()-b->x_pos();
    float delta_y = b->y_pos()-a->y_pos();

    return sqrt(abs(pow(delta_x,2.0f))+abs(pow(delta_y,2.0f)));
}

/*

Car * Util::find_closest_radius(std::vector<Car> &cars, const float x, const float y){
    Car * answer = nullptr;

    float score = 100000;
    for(Car & car : cars){
        float distance = sqrt(abs(pow(car.x_pos()-x,2.0f))+abs(pow(car.y_pos()-y,2.0f)));
        if(distance < score){
            score = distance;
            answer = &car;
        }
    }

    return answer;
}

*/

////////////////////////////////////////////////////////////////////////////////
/// Returns min angle between @param ang1 and @param ang2

float Util::get_min_angle(const float ang1, const float ang2){
    float abs_diff = abs(ang1-ang2);
    float score = std::min(2.0f*(float)M_PI-abs_diff,abs_diff);
    return score;
}

////////////////////////////////////////////////////////////////////////////////
/// Returns distance between two points in 2D.

float Util::distance(float x1, float x2, float y1, float y2) {
    return sqrt(abs(pow(x1-x2,2.0f))+abs(pow(y1-y2,2.0f)));
}