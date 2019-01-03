//
// Created by Carl Schiller on 2018-12-19.
//

#include <iostream>
#include "traffic.h"
#include "window.h"
#include <cmath>


void Simulation::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    if(m_debug){
        // print debug info about node placements and stuff
        sf::CircleShape circle;
        circle.setRadius(4.0f);
        circle.setOutlineColor(sf::Color::Cyan);
        circle.setOutlineThickness(1.0f);
        circle.setFillColor(sf::Color::Transparent);

        sf::Text segment_n;
        segment_n.setFont(m_font);
        segment_n.setFillColor(sf::Color::Black);
        segment_n.setCharacterSize(14);

        sf::VertexArray line(sf::Lines,2);
        line[0].color = sf::Color::Blue;
        line[1].color = sf::Color::Blue;

        int i = 0;
        for(RoadSegment segment: m_traffic.road().segments()){
            for(RoadNode & node : segment.get_nodes()){
                circle.setPosition(sf::Vector2f(node.get_x()*2-4,node.get_y()*2-4));
                line[0].position = sf::Vector2f(node.get_x()*2,node.get_y()*2);
                for(RoadNode * connected_node : node.get_connections()){
                    line[1].position = sf::Vector2f(connected_node->get_x()*2,connected_node->get_y()*2);
                    target.draw(line,states);
                }
                target.draw(circle,states);


            }
            segment_n.setString(std::to_string(i));
            segment_n.setPosition(sf::Vector2f(segment.get_x()*2+4,segment.get_y()*2+4));
            target.draw(segment_n,states);
            i++;
        }
    }

    // one rectangle is all we need :)
    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(9.4,3.4));
    rectangle.setFillColor(sf::Color::Green);
    rectangle.setOutlineColor(sf::Color::Black);
    rectangle.setOutlineThickness(2.0f);

    for(Car * car : m_traffic.get_cars()){
        rectangle.setPosition(car->x_pos()*2,car->y_pos()*2);
        rectangle.setRotation(car->theta()*(float)360.0f/(-2.0f*(float)M_PI));
        sf::Uint8 colorspeed = static_cast<sf::Uint8> ((unsigned int)std::round(255 * car->speed() / car->target_speed()));
        rectangle.setFillColor(sf::Color(255-colorspeed,colorspeed,0,255));
        target.draw(rectangle,states);
    }

}

Simulation::Simulation() {
    m_debug = false;
    m_sim_speed = 1;

    if (!m_font.loadFromFile("/Library/Fonts/Arial.ttf"))
    {
        // error...
    }
}

Simulation::Simulation(bool debug, int speed) {
    m_debug = debug;
    m_sim_speed = speed;

    if (!m_font.loadFromFile("/Library/Fonts/Arial.ttf"))
    {
        // error...
    }
}

void Simulation::update(sf::Time elapsed, double & spawn_counter, double & threshold) {
    float elapsed_time = elapsed.asSeconds();
    for(int i = 0; i < m_sim_speed; i++){
        m_traffic.update(elapsed_time);
        m_traffic.despawn_cars();
        m_traffic.spawn_cars(spawn_counter,elapsed_time,threshold);
    }
}

/*
void Simulation::car_debug(sf::Time t0){
    m_traffic.debug(t0);
}
*/

float Simulation::get_flow() {
    return m_traffic.get_avg_flow();
}

void Simulation::get_info(sf::Text & text,sf::Time &elapsed) {
    float fps = 1.0f/elapsed.asSeconds();
    float flow = get_flow();
    std::string speedy = std::to_string(fps).substr(0,2) +
            " fps, speed: " + std::to_string(m_sim_speed).substr(0,1) + " x\nFlow " +
            std::to_string(get_flow()).substr(0,4);
    text.setString(speedy);
    text.setPosition(0,0);
    text.setFillColor(sf::Color::Green);
    text.setFont(m_font);
}
