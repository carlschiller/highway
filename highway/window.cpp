//
// Created by Carl Schiller on 2018-12-19.
//

#include <iostream>
#include "traffic.h"
#include "window.h"
#include <cmath>


void Car_squares::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    // apply the transform
    states.transform *= getTransform();

    // our particles don't use a texture
    states.texture = &m_texture;

    // draw the vertex array
    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(9.4,3.4));
    rectangle.setFillColor(sf::Color::Green);
    rectangle.setOutlineColor(sf::Color::Black);
    rectangle.setOutlineThickness(2.0f);

    for(Car car : m_traffic.get_cars()){
        rectangle.setPosition(car.x_pos()*2,car.y_pos()*2);
        rectangle.setRotation(car.theta()*(float)360.0f/(-2.0f*(float)M_PI));
        sf::Uint8 colorspeed = static_cast<sf::Uint8> ((unsigned int)std::round(255 * car.vel() / car.target_speed()));
        rectangle.setFillColor(sf::Color(255-colorspeed,colorspeed,0,255));
        target.draw(rectangle,states);
    }
}

Car_squares::Car_squares() {
    m_traffic = Traffic("../kontur.txt");
    //m_traffic.force_spawn_car();
}

void Car_squares::update(sf::Time elapsed, double & spawn_counter, double & threshold, float sim_speed) {
    m_traffic.update(elapsed,sim_speed);
    m_traffic.despawn_cars();
    m_traffic.spawn_cars(spawn_counter,elapsed,threshold,sim_speed);
}


void Car_squares::car_debug(sf::Time t0){
    m_traffic.debug(t0);
}

float Car_squares::get_flow() {
    return m_traffic.get_avg_flow();
}
