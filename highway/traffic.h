//
// Created by Carl Schiller on 2018-12-19.
//
#include <random>
#include <vector>
#include "SFML/Graphics.hpp"

#ifndef HIGHWAY_TRAFFIC_H
#define HIGHWAY_TRAFFIC_H

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
    float m_x_pos, m_y_pos;
    float m_width, m_length;
    float m_vel; // vel and direction
    float m_theta; // radians

    float m_aggressiveness; // how fast to accelerate;
    float m_target_speed;

public:
    Car();
    Car(float x_pos, float y_pos, float vel, float theta, float target_speed, float aggressiveness);

    void update_pos(float delta_t);
    void accelerate(float dtheta);
    void steer(float delta_theta);
    void avoid_collision(std::vector<Car> & cars, int i, sf::Time elapsed);
    float & x_pos();
    float & y_pos();
    float & vel();
    float & target_speed();

    float & width();
    float & length();
    float & theta();
};

enum class Spawn_positions{
    LOWER_LEFT,
    UPPER_RIGHT
};

class Traffic{
private:
    std::vector<std::vector<int>> m_allowed_zone;
    std::vector<Car> m_cars;
    std::map<Spawn_positions,std::vector<float>> m_spawn_positions;

    std::mt19937 & my_engine();

    void update_position(int i, sf::Time & elapsed_time);
    float get_theta(float xpos, float ypos, float speed, float current_theta);
public:
    Traffic();
    explicit Traffic(std::string filename);

    unsigned long n_of_cars();
    void spawn_cars(double & spawn_counter, sf::Time & time, double & threshold);
    void force_spawn_car();
    void debug(sf::Time t0);
    void update(sf::Time & elapsed_time);
    const std::vector<Car> & get_cars()const;
};

#endif //HIGHWAY_TRAFFIC_H
