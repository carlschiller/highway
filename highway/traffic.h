//
// Created by Carl Schiller on 2018-12-19.
//
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
    float m_vel, m_theta; // vel and direction

    float m_front_angle, m_diag;

    float m_aggressiveness; // how fast to accelerate;
    float m_target_speed;

public:
    Car();
    Car(float x_pos, float y_pos, float vel, float theta, float target_speed, float aggressiveness);

    void update_pos(float delta_t);
    void accelerate();
    void steer(float delta_theta);
    float & x_pos();
    float & y_pos();
    float & vel();
    float & target_speed();

    float & width();
    float & length();
    float & theta();
    void plot_edges(sf::Vertex* quad);
};

#endif //HIGHWAY_TRAFFIC_H
