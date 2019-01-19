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
    std::vector<RoadNode*> m_connecting_nodes; // raw pointers, no ownership
    RoadSegment* m_is_child_of; // raw pointer, no ownership
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
    const float m_x, m_y;
    float m_theta;
    const int m_n_lanes;

    constexpr static float M_LANE_WIDTH = 4.0f;

    std::vector<RoadNode*> m_nodes; // OWNERSHIP
    std::vector<Car*> m_cars; // raw pointer, no ownership
    RoadSegment * m_next_segment; // raw pointer, no ownership
public:
    RoadSegment() = delete;
    RoadSegment(float x, float y, RoadSegment * next_segment, int lanes);
    RoadSegment(float x, float y, float theta, int lanes);
    RoadSegment(float x, float y, int lanes,bool merge);
    ~RoadSegment(); // rule of three
    RoadSegment(const RoadSegment&) = delete; // rule of three
    RoadSegment& operator=(const RoadSegment& rhs) = delete; // rule of three

    bool merge;

    RoadNode * get_node_pointer(int n);
    std::vector<RoadNode *> get_nodes();
    void append_car(Car*);
    void remove_car(Car*);
    std::vector<Car*> & get_car_vector();
    RoadSegment * next_segment();
    float get_theta();
    const float get_x() const;
    const float get_y() const;

    int get_lane_number(RoadNode *);
    const int get_total_amount_of_lanes() const;
    void set_theta(float theta);
    void set_next_road_segment(RoadSegment*);
    void calculate_theta();
    void calculate_and_populate_nodes();
    void set_all_node_pointers_to_next_segment();
    void set_node_pointer_to_node(int from_node_n, int to_node_n, RoadSegment *);
};

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
    static Road &shared() {static Road road; return road;}

    Road(const Road& copy) = delete;
    Road& operator=(const Road& rhs) = delete;

    bool load_road();
    std::vector<RoadSegment*> & spawn_positions();
    std::vector<RoadSegment*> & despawn_positions();
    std::vector<RoadSegment*> & segments();
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
    ~Car();
    Car(RoadSegment * spawn_point, int lane, float vel, float target_speed, float agressivness);
    Car(RoadSegment * spawn_point, RoadNode * lane, float vel, float target_speed, float agressivness);

    // all are raw pointers
    RoadSegment * current_segment;
    RoadNode * current_node;
    RoadNode * heading_to_node;
    Car * overtake_this_car;
    std::vector<Car*> want_to_overtake_me;

    std::vector<Car*> & get_overtakers();

    void update_pos(float delta_t);
    void accelerate(float delta_t);
    void avoid_collision(float delta_t);
    Car * find_closest_car();

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
    static bool will_car_paths_cross(Car *a, Car*b);
    static bool is_cars_in_same_lane(Car*a,Car*b);
    static bool merge_helper(Car*a, int merge_to_lane);
    static float distance_to_line(float theta,  float x,  float y);
    static float distance_to_proj_point(float theta,  float x,  float y);
    static float distance_to_car(Car * a, Car * b);
    static Car * find_closest_radius(std::vector<Car> &cars, float x, float y);
    static float get_min_angle(float ang1, float ang2);
    static float distance(float x1, float x2, float y1, float y2);
};

class Traffic : public sf::Drawable, public sf::Transformable{
private:
    std::vector<Car*> m_cars;
    std::mt19937 & my_engine();
    sf::Font m_font;

    //void update_speed(int i, float & elapsed_time);
    //float get_theta(float xpos, float ypos, float speed, float current_theta, bool & lane_switch);
public:
    Traffic();
    ~Traffic();
    Traffic(const Traffic&); // rule of three
    Traffic& operator=(const Traffic&); // rule of three

    unsigned long n_of_cars();
    void spawn_cars(double & spawn_counter, float elapsed, double & threshold);
    void despawn_cars();
    void despawn_all_cars();
    void despawn_car(Car*& car);
    //void remove_car(Car * car);
    void remove_dead_pointers();
    void force_place_car(RoadSegment * seg, RoadNode * node, float vel, float target, float aggro);


    void update(float elapsed_time);
    const std::vector<Car *> & get_cars() const;
    std::vector<Car *> get_car_copies() const;
    float get_avg_flow();

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
public:
    void get_info(sf::Text & text, sf::Time &elapsed);
};

struct car_deleter{
    void operator()(Car*& car);
};


#endif //HIGHWAY_TRAFFIC_H
