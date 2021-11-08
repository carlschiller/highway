//
// Created by Carl Schiller on 2019-03-04.
//

#include "../headers/roadsegment.h"
#include "../headers/roadnode.h"
#include <cmath>
#include <stdexcept>
#include <algorithm>

////////////////////////////////////////////////////////////////////////////////
/// RoadSegment destructor, removes all RodeNode element children because of
/// ownership.

RoadSegment::~RoadSegment(){
    for(RoadNode * elem : m_nodes){
        delete elem;
    }
    m_nodes.clear();
}

////////////////////////////////////////////////////////////////////////////////
/// Constructor, creates a new segment with next connecting segment as
/// @param next_segment

RoadSegment::RoadSegment(float x, float y, RoadSegment * next_segment, int lanes):
        m_x(x),
        m_y(y),
        m_n_lanes(lanes),
        m_next_segment(next_segment)
{
    m_theta = atan2(m_y-m_next_segment->m_y,m_next_segment->m_x-m_x);

    m_nodes.reserve(m_n_lanes);

    ramp_counter = 0;
    car_passed = false;
    meter = false;
    period = 0;

    calculate_and_populate_nodes(); // populates segment with RoadNodes.
}

////////////////////////////////////////////////////////////////////////////////
/// Constructor, creates a new segment with manually entered @param theta.

RoadSegment::RoadSegment(float x, float y, float theta, int lanes) :
        m_x(x),
        m_y(y),
        m_theta(theta),
        m_n_lanes(lanes),
        m_next_segment(nullptr)
{
    m_nodes.reserve(m_n_lanes);

    ramp_counter = 0;
    car_passed = false;
    meter = false;
    period = 0;

    calculate_and_populate_nodes(); // populates segment with RoadNodes.
}

////////////////////////////////////////////////////////////////////////////////
/// Constructor, creates a new segment without creating RoadNodes. This
/// needs to be done manually with functions below.

RoadSegment::RoadSegment(float x, float y, int lanes, bool mer):
        m_x(x),
        m_y(y),
        m_n_lanes(lanes),
        m_next_segment(nullptr),
        merge(mer)
{
    m_nodes.reserve(m_n_lanes);

    ramp_counter = 0;
    car_passed = false;
    meter = false;
    period = 0;

    // can't set nodes if we don't have a theta.
}

////////////////////////////////////////////////////////////////////////////////
/// Returns theta (angle) of RoadSegment, in which direction the segment points

float RoadSegment::get_theta() {
    return m_theta;
}

////////////////////////////////////////////////////////////////////////////////
/// Returns x position of RoadSegment.

const float RoadSegment::get_x() const{
    return m_x;
}

////////////////////////////////////////////////////////////////////////////////
/// Returns y position of RoadSegment.

const float RoadSegment::get_y() const {
    return m_y;
}

////////////////////////////////////////////////////////////////////////////////
/// Returns int number of @param node. E.g. 0 would be the right-most lane.
/// Throws exception if we do not find the node in this segment.

int RoadSegment::get_lane_number(RoadNode * node) {
    for(int i = 0; i < m_n_lanes; i++){
        if(node == m_nodes[i]){
            return i;
        }
    }
    throw std::invalid_argument("Node is not in this segment");
}

////////////////////////////////////////////////////////////////////////////////
/// Adds a new car to the segment.

void RoadSegment::append_car(Car * car) {
    m_cars.push_back(car);
}

////////////////////////////////////////////////////////////////////////////////
/// Removes car from segment, if car is not in list we do nothing.

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

    /*
    if(!found){
        throw std::invalid_argument("Car is not in this segment.");
    }
    */
}

////////////////////////////////////////////////////////////////////////////////
/// Sets theta of RoadSegment according to @param theta.

void RoadSegment::set_theta(float theta) {
    m_theta = theta;
}

////////////////////////////////////////////////////////////////////////////////
/// Automatically populates segment with nodes according to amount of lanes
/// specified and theta specified.

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

////////////////////////////////////////////////////////////////////////////////
/// Sets next segment to @param next_segment

void RoadSegment::set_next_road_segment(RoadSegment * next_segment) {
    m_next_segment = next_segment;
}

////////////////////////////////////////////////////////////////////////////////
/// Calculates theta according to next_segment. Throws if m_next_segment is
/// nullptr

void RoadSegment::calculate_theta() {
    if(m_next_segment == nullptr){
        throw std::invalid_argument("Can't calculate theta if next segment is nullptr");
    }
    m_theta = atan2(m_y-m_next_segment->m_y,m_next_segment->m_x-m_x);
}

////////////////////////////////////////////////////////////////////////////////
/// Returns node of lane number n. E.g. n=0 is the right-most lane.

RoadNode* RoadSegment::get_node_pointer(int n) {
    return m_nodes[n];
}

////////////////////////////////////////////////////////////////////////////////
/// Returns all nodes in segment.

std::vector<RoadNode *> RoadSegment::get_nodes() {
    return m_nodes;
}

////////////////////////////////////////////////////////////////////////////////
/// Returns next segment

RoadSegment* RoadSegment::next_segment() {
    return m_next_segment;
}

////////////////////////////////////////////////////////////////////////////////
/// Automatically populates node connections by connecting current node to
/// all nodes in next segment.

void RoadSegment::set_all_node_pointers_to_next_segment() {
    for(RoadNode * node: m_nodes){
        for(int i = 0; i < m_next_segment->m_n_lanes; i++){
            node->set_next_node(m_next_segment->get_node_pointer(i));
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
/// Manually set connection to next segment's node. No guarantee is made
/// on @param from_node_n and @param to_node_n. Can crash if index out of range.

void RoadSegment::set_node_pointer_to_node(int from_node_n, int to_node_n, RoadSegment *next_segment) {
    RoadNode * pointy = next_segment->get_node_pointer(to_node_n);
    m_nodes[from_node_n]->set_next_node(pointy);
}

////////////////////////////////////////////////////////////////////////////////
/// Returns amount of lanes in this segment.

const int RoadSegment::get_total_amount_of_lanes() const {
    return m_n_lanes;
}
