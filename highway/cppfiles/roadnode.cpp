//
// Created by Carl Schiller on 2019-03-04.
//

#include "../headers/roadnode.h"
#include <cmath>

////////////////////////////////////////////////////////////////////////////////
/// Constructor

RoadNode::RoadNode() = default;

////////////////////////////////////////////////////////////////////////////////
/// Destructor

RoadNode::~RoadNode() = default;

////////////////////////////////////////////////////////////////////////////////
/// Constructor, @param x is x position of node, @param y is y position of node,
/// @param segment is to which segment this RoadNode belongs.

RoadNode::RoadNode(float x, float y, RoadSegment * segment) {
    m_x = x;
    m_y = y;
    m_is_child_of = segment;
}

////////////////////////////////////////////////////////////////////////////////
/// Appends a new RoadNode to the list connections from this RoadNode.
/// I.e. to where a Car is allowed to drive.

void RoadNode::set_next_node(RoadNode * next_node) {
    m_nodes_from_me.push_back(next_node);
    next_node->m_nodes_to_me.push_back(this); // sets double linked chain.
}

////////////////////////////////////////////////////////////////////////////////
/// Appends a new RoadNode to the list connections to this RoadNode.
/// I.e. from where a Car is allowed to drive to this Node.

void RoadNode::set_previous_node(RoadNode * prev_node) {
    m_nodes_to_me.push_back(prev_node);
}

////////////////////////////////////////////////////////////////////////////////
/// Returns RoadSegment to which this RoadNode belongs.

RoadSegment* RoadNode::get_parent_segment() {
    return m_is_child_of;
}

////////////////////////////////////////////////////////////////////////////////
/// Returns connections from this RoadNode.

std::vector<RoadNode*> & RoadNode::get_nodes_from_me() {
    return m_nodes_from_me;
}

////////////////////////////////////////////////////////////////////////////////
/// Returns connections to this RoadNode.

std::vector<RoadNode*>& RoadNode::get_nodes_to_me() {
    return m_nodes_to_me;
}

////////////////////////////////////////////////////////////////////////////////
/// Returns x position of RoadNode.

float RoadNode::get_x() {
    return m_x;
}

////////////////////////////////////////////////////////////////////////////////
/// Returns y position of RoadNode.

float RoadNode::get_y() {
    return m_y;
}

////////////////////////////////////////////////////////////////////////////////
/// Returns angle of this RoadNode to @param node as a mathematitian
/// would define angles. In radians.

float RoadNode::get_theta(RoadNode* node) {
    for(RoadNode * road_node : m_nodes_from_me){
        if(node == road_node){
            return atan2(m_y-node->m_y,node->m_x-m_x);
        }
    }
    throw std::invalid_argument("Node given is not a connecting node");
}

////////////////////////////////////////////////////////////////////////////////
/// Returns RoadNode according to @param lane from the vector of node
/// connections from this RoadNode.

RoadNode* RoadNode::get_next_node(int lane) {
    return m_nodes_from_me[lane];
}

