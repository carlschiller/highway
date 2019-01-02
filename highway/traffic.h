//
// Created by Carl Schiller on 2018-12-19.
//
#include <random>
#include <vector>
#include "SFML/Graphics.hpp"

#ifndef HIGHWAY_TRAFFIC_H
#define HIGHWAY_TRAFFIC_H



class RoadSegment;

class Car;

class RoadNode{
private:
    float m_x, m_y;
    std::vector<RoadNode*> m_connecting_nodes;
    RoadSegment* m_is_child_of;
public:
    RoadNode();
    ~RoadNode();
    RoadNode(float x, float y, RoadSegment * segment);

    void set_pointer(RoadNode*);
    RoadSegment* get_parent_segment();
    RoadNode * get_next_node(int lane);
    std::vector<RoadNode*> & get_connections();
    float get_x();
    float get_y();
    float get_theta(RoadNode*);
};


class RoadSegment{
private:
    float m_x, m_y, m_theta;
    int m_n_lanes;
    constexpr static float M_LANE_WIDTH = 4.0f;

    std::vector<RoadNode> m_nodes;
    std::map<int,bool> m_car_ids;
    RoadSegment * m_next_segment;
public:
    RoadSegment();
    RoadSegment(float x, float y, RoadSegment * next_segment, int lanes);
    RoadSegment(float x, float y, float theta, int lanes);
    RoadSegment(float x, float y, int lanes);
    ~RoadSegment();

    RoadNode * get_node_pointer(int n);
    std::vector<RoadNode> & get_nodes();
    void append_car(Car*);
    void remove_car(Car*);
    std::map<int,bool> & get_car_map();
    RoadSegment * next_segment();
    float get_theta();
    float get_x();
    float get_y();

    int get_lane_number(RoadNode *);
    void set_theta(float theta);
    void set_next_road_segment(RoadSegment*);
    void calculate_theta();
    void calculate_and_populate_nodes();
    void set_all_node_pointers_to_next_segment();
    void set_node_pointer_to_node(int from_node_n, int to_node_n, RoadSegment *);
};

class Road{
private:
    std::vector<RoadSegment> m_segments;
    std::vector<RoadSegment*> m_spawn_positions;
    std::vector<RoadSegment*> m_despawn_positions;

    const std::string M_FILENAME = "../road.txt";
public:
    Road();
    ~Road();

    void insert_segment(RoadSegment &);
    bool load_road();
    std::vector<RoadSegment*> & spawn_positions();
    std::vector<RoadSegment*> & despawn_positions();
    const std::vector<RoadSegment> & segments()const;
};

/**
 * Car class
 * =========
 * Private:
 * position, width of car, and velocities are stored.
 * -------------------
 * Public:
 * .update_pos(float delta_t): updates position by updating position.
 * .accelerate(float delta_v): accelerates car.
 * .steer(float delta_theta): change direction of speed.
 * .x_pos(): return reference to x_pos.
 * .y_pos(): -||- y_pos.
 */

class Car{
private:
    float m_dist_to_next_node;
    float m_speed;
    float m_theta; // radians

    float m_aggressiveness; // how fast to accelerate;
    float m_target_speed;
    bool m_breaking;

public:
    Car();
    Car(RoadSegment * spawn_point, int lane, float vel, float target_speed, float agressivness, int unique_id);

    int id;

    RoadSegment * current_segment;
    RoadNode * current_node;
    RoadNode * heading_to_node;

    void update_pos(float delta_t);
    void accelerate();

    //void avoid_collision(std::vector<Car> & cars, int i, float & elapsed,float delta_theta,
    //                     std::vector<std::vector<int>> & allowed_zon);

    float x_pos();
    float y_pos();

    float & speed();
    float & target_speed();
    float & theta();

    RoadSegment * get_segment();
};


class Util{
public:
    static std::vector<std::string> split_string_by_delimiter(const std::string & str, const char delim);
    static bool is_car_behind(Car * a, Car * b);
    static float distance_to_line(float theta,  float x,  float y);
    static float distance_to_proj_point( float theta,  float x,  float y);
    static float distance_to_car(Car & a, Car & b);
    static bool find_connected_path(Car & ref, Car & car, std::vector<std::vector<int>> & allowed_zone, int buffer);
    static Car * find_closest_car(std::vector<Car> &cars, Car * ref, std::vector<std::vector<int>> & allowed_zone);
    static Car * find_closest_radius(std::vector<Car> &cars, float x, float y);
    static float get_min_angle(float ang1, float ang2);
    static float distance(float x1, float x2, float y1, float y2);
};

class Traffic{
private:
    Road m_road = Road();
    std::vector<Car> m_cars;
    int m_id;

    std::mt19937 & my_engine();

    //void update_speed(int i, float & elapsed_time);
    //float get_theta(float xpos, float ypos, float speed, float current_theta, bool & lane_switch);
public:
    Traffic();

    const unsigned long n_of_cars()const;
    const Road & road()const;
    void spawn_cars(double & spawn_counter, float elapsed, double & threshold);
    void despawn_cars();
    //void force_spawn_car();
    void debug(sf::Time t0);
    void update(float elapsed_time);
    const std::vector<Car> & get_cars()const;
    float get_avg_flow();
};

#endif //HIGHWAY_TRAFFIC_H
