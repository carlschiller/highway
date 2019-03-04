//
// Created by Carl Schiller on 2019-03-04.
//

#include "../headers/roadsegment.h"
#include "../headers/roadnode.h"
#include <cmath>

// Roadsegment below

RoadSegment::~RoadSegment(){
    for(RoadNode * elem : m_nodes){
        delete elem;
    }
    m_nodes.clear();
}

RoadSegment::RoadSegment(float x, float y, RoadSegment * next_segment, int lanes):
        m_x(x), m_y(y), m_n_lanes(lanes)
{
    m_next_segment = next_segment;
    m_theta = atan2(m_y-m_next_segment->m_y,m_next_segment->m_x-m_x);

    m_nodes.reserve(m_n_lanes);

    calculate_and_populate_nodes();
}

RoadSegment::RoadSegment(float x, float y, float theta, int lanes) :
        m_x(x), m_y(y), m_theta(theta), m_n_lanes(lanes)
{
    m_next_segment = nullptr;
    m_nodes.reserve(lanes);

    calculate_and_populate_nodes();
}

RoadSegment::RoadSegment(float x, float y, int lanes,bool mer):
        m_x(x), m_y(y), merge(mer), m_n_lanes(lanes)
{
    merge = mer;
    m_next_segment = nullptr;
    m_nodes.reserve(m_n_lanes);

    // can't set nodes if we don't have a theta.
}

float RoadSegment::get_theta() {
    return m_theta;
}

const float RoadSegment::get_x() const{
    return m_x;
}

const float RoadSegment::get_y() const {
    return m_y;
}

int RoadSegment::get_lane_number(RoadNode * node) {
    for(int i = 0; i < m_n_lanes; i++){
        if(node == m_nodes[i]){
            return i;
        }
    }
    throw std::invalid_argument("Node is not in this segment");
}

void RoadSegment::append_car(Car * car) {
    m_cars.push_back(car);
}

void RoadSegment::remove_car(Car * car) {
    unsigned long size = m_cars.size();
    bool found = false;
    for(int i = 0; i < size; i++){
        if(car == m_cars[i]){
            m_cars[i] = nullptr;
            found = true;
        }
    }
    std::vector<Car*>::iterator new_end = std::remove(m_cars.begin(),m_cars.end(),static_cast<Car*>(nullptr));
    m_cars.erase(new_end,m_cars.end());

    if(!found){
        //throw std::invalid_argument("Car is not in this segment.");
    }
}

void RoadSegment::set_theta(float theta) {
    m_theta = theta;
}

void RoadSegment::calculate_and_populate_nodes() {
    // calculates placement of nodes.
    float total_length = M_LANE_WIDTH*(m_n_lanes-1);
    float current_length = -total_length/2.0f;

    for(int i = 0; i < m_n_lanes; i++){
        float x_pos = m_x+current_length*cos(m_theta+(float)M_PI*0.5f);
        float y_pos = m_y-current_length*sin(m_theta+(float)M_PI*0.5f);
        m_nodes.push_back(new RoadNode(x_pos,y_pos,this));
        current_length += M_LANE_WIDTH;
    }
}

void RoadSegment::set_next_road_segment(RoadSegment * next_segment) {
    m_next_segment = next_segment;
}


void RoadSegment::calculate_theta() {
    m_theta = atan2(m_y-m_next_segment->m_y,m_next_segment->m_x-m_x);
}

RoadNode* RoadSegment::get_node_pointer(int n) {
    return m_nodes[n];
}

std::vector<RoadNode *> RoadSegment::get_nodes() {
    return m_nodes;
}

RoadSegment* RoadSegment::next_segment() {
    return m_next_segment;
}


void RoadSegment::set_all_node_pointers_to_next_segment() {
    for(RoadNode * node: m_nodes){
        for(int i = 0; i < m_next_segment->m_n_lanes; i++){
            node->set_next_node(m_next_segment->get_node_pointer(i));
        }
    }
}

void RoadSegment::set_node_pointer_to_node(int from_node_n, int to_node_n, RoadSegment *next_segment) {
    RoadNode * pointy = next_segment->get_node_pointer(to_node_n);
    m_nodes[from_node_n]->set_next_node(pointy);
}

const int RoadSegment::get_total_amount_of_lanes() const {
    return m_n_lanes;
}
