//
// Created by Carl Schiller on 2018-12-19.
//

#include <iostream>
#include "traffic.h"
#include "window.h"
#include <cmath>
#include <unistd.h>

/*
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

        for(RoadSegment * segment : Road::shared().segments()){
            for(RoadNode * node : segment->get_nodes()){
                circle.setPosition(sf::Vector2f(node->get_x()*2-4,node->get_y()*2-4));
                line[0].position = sf::Vector2f(node->get_x()*2,node->get_y()*2);
                for(RoadNode * connected_node : node->get_connections()){
                    line[1].position = sf::Vector2f(connected_node->get_x()*2,connected_node->get_y()*2);
                    target.draw(line,states);
                }
                target.draw(circle,states);


            }
            segment_n.setString(std::to_string(i));
            segment_n.setPosition(sf::Vector2f(segment->get_x()*2+4,segment->get_y()*2+4));
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
    for(Car * car : m_traffic.get_car_copies()){
        rectangle.setPosition(car->x_pos()*2,car->y_pos()*2);
        rectangle.setRotation(car->theta()*(float)360.0f/(-2.0f*(float)M_PI));
        sf::Uint8 colorspeed = static_cast<sf::Uint8> ((unsigned int)std::round(255 * car->speed() / car->target_speed()));
        rectangle.setFillColor(sf::Color(255-colorspeed,colorspeed,0,255));
        target.draw(rectangle,states);

        // print debug info about node placements and stuff

        if(car->heading_to_node!=nullptr){
            sf::CircleShape circle;
            circle.setRadius(4.0f);
            circle.setOutlineColor(sf::Color::Red);
            circle.setOutlineThickness(2.0f);
            circle.setFillColor(sf::Color::Transparent);
            circle.setPosition(sf::Vector2f(car->current_node->get_x()*2-4,car->current_node->get_y()*2-4));
            target.draw(circle,states);
            circle.setOutlineColor(sf::Color::Green);
            circle.setPosition(sf::Vector2f(car->heading_to_node->get_x()*2-4,car->heading_to_node->get_y()*2-4));
            target.draw(circle,states);
        }
    }
}
*/


Simulation::Simulation(Traffic *&traffic, sf::Mutex *&mutex, int sim_speed, int framerate, bool *& exit_bool):
    M_FRAMERATE(framerate),
    M_SIM_SPEED(sim_speed),
    m_traffic(traffic),
    m_mutex(mutex),
    m_exit_bool(exit_bool)
{

}

void Simulation::update() {
    sf::Clock clock;
    sf::Time time;
    double spawn_counter = 0.0;
    double threshold = 0.0;

    while(!*m_exit_bool){
        m_mutex->lock();
        std::cout << "calculating\n";
        for(int i = 0; i < M_SIM_SPEED; i++){
            std::cout<< "a\n";
            m_traffic->update(1.0f/(float)M_FRAMERATE);
            std::cout<< "b\n";
            m_traffic->spawn_cars(spawn_counter,1.0f/(float)M_FRAMERATE,threshold);
            std::cout<< "c\n";
            m_traffic->despawn_cars();
            std::cout<< "d\n";
        }
        std::cout << "calculated\n";
        m_mutex->unlock();

        time = clock.restart();
        sf::Int64 acutal_elapsed = time.asMicroseconds();
        double sim_elapsed = 1.0f/(float)M_FRAMERATE*1000000;
        /*
        if(acutal_elapsed < sim_elapsed){
            usleep((useconds_t)(sim_elapsed-acutal_elapsed));
        }
        */
    }
}

/*
void Simulation::car_debug(sf::Time t0){
    m_traffic.debug(t0);
}
*/