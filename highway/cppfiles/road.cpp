//
// Created by Carl Schiller on 2019-03-04.
//

#include "../headers/road.h"
#include <fstream>
#include <vector>
#include "../headers/roadsegment.h"
#include <iostream>
#include "../headers/util.h"

////////////////////////////////////////////////////////////////////////////////
/// Constructor of Road.

Road::Road() :
        M_FILENAME("../road.txt")
{
    if(!load_road()){
        std::cout << "Error in loading road.\n";
    };
}

////////////////////////////////////////////////////////////////////////////////
/// Destructor of Road.

Road::~Road() {
    for(RoadSegment * seg : m_segments){
        delete seg;
    }
    m_segments.clear();
}

////////////////////////////////////////////////////////////////////////////////
/// Function to load Road from txt file. Parsing as follows:
///
/// # ignores current line input.
///
/// If there are 4 tokens in current line:
/// tokens[0]: segment number
/// tokens[1]: segment x position
/// tokens[2]: segment y position
/// tokens[3]: amount of lanes
///
/// If there are 5 tokens in current line:
/// tokens[0]: segment number
/// tokens[1]: segment x position
/// tokens[2]: segment y position
/// tokens[3]: amount of lanes
/// tokens[4]: spawn point or if it's a merging lane (true/false/merge)
///
/// If there are 4+3*n tokens in current line:
/// tokens[0]: segment number
/// tokens[1]: segment x position
/// tokens[2]: segment y position
/// tokens[3]: amount of lanes
/// tokens[3+3*n]: from lane number of current segment
/// tokens[4+3*n]: to lane number of segment specified in next token (below)
/// tokens[5+3*n]: to segment number.

bool Road::load_road() {
    bool loading = true;
    std::ifstream stream;
    stream.open(M_FILENAME);

    std::vector<std::vector<std::string>> road_vector;
    road_vector.reserve(100);

    if(stream.is_open()){
        std::string line;
        std::vector<std::string> tokens;
        while(std::getline(stream,line)){
            tokens = Util::split_string_by_delimiter(line,' ');
            if(tokens[0] != "#"){
                road_vector.push_back(tokens);
            }
        }
    }
    else{
        loading = false;
    }


    // load segments into memory.
    for(std::vector<std::string> & vec : road_vector){
        if(vec.size() == 5){
            if(vec[4] == "merge"){
                RoadSegment * seg = new RoadSegment(std::stof(vec[1]),std::stof(vec[2]),std::stoi(vec[3]),true);
                m_segments.push_back(seg);
            }
            else{
                RoadSegment * seg = new RoadSegment(std::stof(vec[1]),std::stof(vec[2]),std::stoi(vec[3]),false);
                m_segments.push_back(seg);
            }
        }
        else{
            RoadSegment * seg = new RoadSegment(std::stof(vec[1]),std::stof(vec[2]),std::stoi(vec[3]),false);
            m_segments.push_back(seg);
        }

    }


    // populate nodes.
    for (int i = 0; i < m_segments.size(); ++i) {
        // populate nodes normally.
        if(road_vector[i].size() == 4){
            m_segments[i]->set_next_road_segment(m_segments[i+1]);
            m_segments[i]->calculate_theta();
            // calculate nodes based on theta.
            m_segments[i]->calculate_and_populate_nodes();

        }
        else if(road_vector[i].size() == 5){
            if(road_vector[i][4] == "false"){
                // take previous direction and populate nodes.
                m_segments[i]->set_theta(m_segments[i-1]->get_theta());
                m_segments[i]->calculate_and_populate_nodes();
                // but do not connect nodes to new ones.

                // make this a despawn segment
                m_despawn_positions.push_back(m_segments[i]);
            }
            else if(road_vector[i][4] == "true"){
                m_segments[i]->set_next_road_segment(m_segments[i+1]);
                m_segments[i]->calculate_theta();
                // calculate nodes based on theta.
                m_segments[i]->calculate_and_populate_nodes();

                // make this a spawn segment
                m_spawn_positions.push_back(m_segments[i]);
            }
            else if(road_vector[i][4] == "merge"){
                m_segments[i]->set_next_road_segment(m_segments[i+1]);
                m_segments[i]->calculate_theta();
                // calculate nodes based on theta.
                m_segments[i]->calculate_and_populate_nodes();
            }

        }
            // else we connect one by one.
        else{
            // take previous direction and populate nodes.
            m_segments[i]->set_theta(m_segments[i-1]->get_theta());
            // calculate nodes based on theta.
            m_segments[i]->calculate_and_populate_nodes();
        }
    }

    // connect nodes.
    for (int i = 0; i < m_segments.size(); ++i) {
        // do normal connection, ie connect all nodes.
        if(road_vector[i].size() == 4){
            m_segments[i]->set_all_node_pointers_to_next_segment();
        }
        else if(road_vector[i].size() == 5){
            if(road_vector[i][4] == "false"){
                // but do not connect nodes to new ones.
            }
            else if(road_vector[i][4] == "true"){
                m_segments[i]->set_all_node_pointers_to_next_segment();
            }
            else if(road_vector[i][4] == "merge"){
                m_segments[i]->set_all_node_pointers_to_next_segment();
            }

        }
            // else we connect one by one.
        else{
            // manually connect nodes.
            int amount_of_pointers = (int)road_vector[i].size()-4;
            for(int j = 0; j < amount_of_pointers/3; j++){
                int current_pos = 4+j*3;
                RoadSegment * next_segment = m_segments[std::stoi(road_vector[i][current_pos+2])];
                m_segments[i]->set_node_pointer_to_node(std::stoi(road_vector[i][current_pos]),std::stoi(road_vector[i][current_pos+1]),next_segment);
            }
        }
    }
    return loading;
}

////////////////////////////////////////////////////////////////////////////////
/// Returns spawn positions of Road

std::vector<RoadSegment*>& Road::spawn_positions() {
    return m_spawn_positions;
}

////////////////////////////////////////////////////////////////////////////////
/// Returns despawn positions of Road

std::vector<RoadSegment*>& Road::despawn_positions() {
    return m_despawn_positions;
}

////////////////////////////////////////////////////////////////////////////////
/// Returns all segments of Road.

std::vector<RoadSegment*>& Road::segments() {
    return m_segments;
}
