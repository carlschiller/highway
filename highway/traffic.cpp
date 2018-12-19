//
// Created by Carl Schiller on 2018-12-19.
//

#include "traffic.h"
#include <cmath>

Car::Car() = default;

Car::Car(float x_pos, float y_pos, float vel, float theta, float target_speed, float aggressiveness) {
    // volvo v70 dimensions.
    m_length = 4.7;
    m_width = 1.7;

    m_x_pos = x_pos;
    m_y_pos = y_pos;
    m_vel = vel;
    m_theta = theta;

    // target speed and how fast to reach speed.
    m_target_speed = target_speed;
    m_aggressiveness = aggressiveness;

    m_front_angle = atan(m_width/m_length);
    m_diag = sqrt(pow(m_length * 0.5f, 2.0f) + pow(m_width * 0.5f, 2.0f));
}

void Car::update_pos(float delta_t) {
    m_x_pos += cos(m_theta)*m_vel*delta_t;
    m_y_pos += sin(m_theta)*m_vel*delta_t;
}

void Car::accelerate(){
    float d_vel; // proportional control.
    if(m_vel < m_target_speed*0.75){
        d_vel = m_aggressiveness;
    }
    else{
        d_vel = m_aggressiveness*(m_target_speed-m_vel)*4;
    }

    m_vel += d_vel;
}

void Car::steer(float delta_theta) {
    m_theta += delta_theta;
}

float & Car::x_pos() {
    return m_x_pos;
}

float & Car::y_pos() {
    return m_y_pos;
}

float & Car::vel() {
    return m_vel;
}

float & Car::target_speed() {
    return m_target_speed;
}

float & Car::length() { return m_length; }

float & Car::width() { return m_width; }

float & Car::theta() { return m_theta; }

void Car::plot_edges(sf::Vertex *quad) {
    quad[0].position = sf::Vector2f((m_x_pos + m_diag * cos(m_front_angle + m_theta))*4,
                                    (m_y_pos + m_diag * sin(m_front_angle + m_theta))*4);
    quad[1].position = sf::Vector2f((m_x_pos + m_diag * cos(-m_front_angle + m_theta))*4,
                                    (m_y_pos + m_diag * sin(-m_front_angle + m_theta))*4);
    quad[2].position = sf::Vector2f((m_x_pos + m_diag * cos(m_front_angle + m_theta-3.141f))*4,
                                    (m_y_pos + m_diag * sin(m_front_angle + m_theta-3.141f))*4);
    quad[3].position = sf::Vector2f((m_x_pos + m_diag * cos(-m_front_angle + m_theta-3.141f))*4,
                                    (m_y_pos + m_diag * sin(-m_front_angle + m_theta-3.141f))*4);
}

