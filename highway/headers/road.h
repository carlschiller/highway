//
// Created by Carl Schiller on 2019-03-04.
//

#ifndef HIGHWAY_ROAD_H
#define HIGHWAY_ROAD_H

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// Road                                                                 //
//                                                                      //
// Describes a road with interconnected nodes. Mathematically it is     //
// a graph.                                                             //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "roadsegment.h"
#include <vector>
#include <string>

class Road{
private:
    std::vector<RoadSegment*> m_segments; // OWNERSHIP
    std::vector<RoadSegment*> m_spawn_positions; // raw pointers
    std::vector<RoadSegment*> m_despawn_positions; // raw pointers

    const std::string M_FILENAME;
private:
    Road();
    ~Road();
public:
    static Road &shared() {static Road road; return road;} // in order to only load road once in memory

    Road(const Road& copy) = delete; // no copying allowed
    Road& operator=(const Road& rhs) = delete; // no copying allowed

    bool load_road();
    std::vector<RoadSegment*> & spawn_positions();
    std::vector<RoadSegment*> & despawn_positions();
    std::vector<RoadSegment*> & segments();
};

#endif //HIGHWAY_ROAD_H
