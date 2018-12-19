//
// Created by Carl Schiller on 2018-12-19.
//

#include <iostream>
#include "traffic.h"
#include "window.h"
#include <cmath>


void Car_squares::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    {
        // apply the transform
        states.transform *= getTransform();

        // our particles don't use a texture
        states.texture = &m_texture;

        // draw the vertex array
        target.draw(m_vertices, states);
    }
}

Car_squares::Car_squares() {
    m_cars.emplace_back(Car(100,100,0,3.141f/4.0f,20,0.02f));
    m_vertices = sf::VertexArray(sf::Quads,4);
    for(int i = 0; i < m_cars.size()*4; i++){
        m_vertices[i].color = sf::Color(0,0,0,255);
    }
}

void Car_squares::update(sf::Time elapsed) {
    for(int i = 0; i < m_cars.size(); i++){
        Car &car  = m_cars[i];
        car.accelerate();
        car.steer(0.01f);
        car.update_pos(elapsed.asSeconds());

        sf::Vertex* quad = &m_vertices[i];
        car.plot_edges(quad);
    }
}

void Car_squares::car_debug(sf::Time t0){
    std::string message;
    message += "Vel: " + std::to_string(m_cars[0].vel()*3.6f).substr(0,4) + " km/h, time: " + std::to_string(t0.asSeconds()).substr(0,3);
    std::cout << message << std::endl;
}