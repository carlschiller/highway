//
// Created by Carl Schiller on 2019-03-04.
//

#ifndef HIGHWAY_ROADNODE_H
#define HIGHWAY_ROADNODE_H

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// RoadNode                                                             //
//                                                                      //
// Describes the smallest element in Road, it is similar to             //
// that of a mathematical graph with nodes and edges.                   //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include <vector>
#include "car.h"
#include "roadsegment.h"

class RoadNode{
private:
    float m_x, m_y;
    std::vector<RoadNode*> m_nodes_from_me; // raw pointers, no ownership
    std::vector<RoadNode*> m_nodes_to_me;
    RoadSegment* m_is_child_of; // raw pointer, no ownership
public:
    RoadNode();
    ~RoadNode();
    RoadNode(float x, float y, RoadSegment * segment);

    void set_next_node(RoadNode *);
    void set_previous_node(RoadNode *);
    RoadSegment* get_parent_segment();
    RoadNode * get_next_node(int lane);
    std::vector<RoadNode*> & get_nodes_from_me();
    std::vector<RoadNode*> & get_nodes_to_me();
    float get_x();
    float get_y();
    float get_theta(RoadNode*);
};


#endif //HIGHWAY_ROADNODE_H
