//
// Created by Carl Schiller on 2019-01-16.
//

#include "unittests.h"
#include <unistd.h>
#include <iostream>

void Tests::placement_test() {
    std::cout << "Starting placement tests\n";
    std::vector<RoadSegment*> segments = Road::shared().segments();
    int i = 0;

    for(RoadSegment * seg : segments){
        usleep(100000);
        std::cout<< "seg " << i << ", nlanes " << seg->get_total_amount_of_lanes() << ","<< seg << std::endl;
        std::cout << "next segment" << seg->next_segment() << std::endl;
        std::vector<RoadNode*> nodes =  seg->get_nodes();
        for(RoadNode * node : nodes){
            std::vector<RoadNode*> connections = node->get_connections();
            std::cout << "node" << node <<" has connections:" <<  std::endl;
            for(RoadNode * pointy : connections){
                std::cout << pointy << std::endl;
            }
        }
        i++;
        m_traffic->force_place_car(seg,seg->get_nodes()[0],1,1,0.01);
        std::cout << "placed car" << std::endl;
    }
    std::cout << "Placement tests passed\n";
}

void Tests::delete_cars_test() {
    std::vector<Car*> car_copies = m_traffic->get_car_copies();

    for(Car * car : car_copies){
        std::cout << car << std::endl;
        usleep(100);
        m_mutex->lock();
        std::cout << "deleting car\n";
        //usleep(100000);
        //std::cout << "Removing car " << car << std::endl;
        m_traffic->despawn_car(car);
        m_mutex->unlock();
        std::cout << car << std::endl;
    }
    std::cout << "Car despawn tests passed\n";
}

void Tests::run_one_car() {
    double ten = 10.0;
    double zero = 0;
    m_traffic->spawn_cars(ten,0,zero);
    double fps = 60.0;
    double multiplier = 10.0;

    std::cout << "running one car\n";
    while(m_traffic->n_of_cars() != 0) {
        usleep((useconds_t)(1000000.0/(fps*multiplier)));
        m_traffic->update(1.0f/(float)fps);
        m_traffic->despawn_cars();
    }
}

void Tests::placement_test_2() {
    std::cout << "Starting placement tests 2\n";
    std::vector<RoadSegment*> segments = Road::shared().segments();
    int i = 0;

    for(RoadSegment * seg : segments){
        usleep(100000);
        std::cout<< "seg " << i << ", nlanes " << seg->get_total_amount_of_lanes() << ","<< seg << std::endl;
        std::cout << "next segment" << seg->next_segment() << std::endl;
        std::vector<RoadNode*> nodes =  seg->get_nodes();
        for(RoadNode * node : nodes){
            std::vector<RoadNode*> connections = node->get_connections();
            std::cout << "node" << node <<" has connections:" <<  std::endl;
            for(RoadNode * pointy : connections){
                std::cout << pointy << std::endl;
            }
            m_traffic->force_place_car(seg,node,1,1,0.1);
            std::cout << "placed car"  << std::endl;
        }
        i++;

    }
    m_traffic->despawn_all_cars();
    std::cout << "Placement tests 2 passed\n";
}

void Tests::placement_test_3() {
    std::cout << "Starting placement tests 3\n";
    std::vector<RoadSegment*> segments = Road::shared().segments();

    for (int i = 0; i < 10000; ++i) {
        usleep(100);
        m_traffic->force_place_car(segments[0],segments[0]->get_nodes()[0],1,1,1);
    }

    delete_cars_test();
    //m_traffic.despawn_all_cars();
    std::cout << "Placement tests 3 passed\n";
}


// do all tests
void Tests::run_all_tests() {
    usleep(2000000);
    placement_test();
    delete_cars_test();
    run_one_car();
    placement_test_2();
    placement_test_3();

    std::cout << "all tests passed\n";
}
/*
void Tests::draw(sf::RenderTarget &target, sf::RenderStates states) const {
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

    // one rectangle is all we need :)
    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(9.4,3.4));
    rectangle.setFillColor(sf::Color::Green);
    rectangle.setOutlineColor(sf::Color::Black);
    rectangle.setOutlineThickness(2.0f);

    std::cout << "start drawing\n";
    for(Car * car : m_traffic.get_cars()){
        std::cout << "drawing" << car << std::endl;
        if(car != nullptr){
            rectangle.setPosition(car->x_pos()*2,car->y_pos()*2);
            rectangle.setRotation(car->theta()*(float)360.0f/(-2.0f*(float)M_PI));
            sf::Uint8 colorspeed = static_cast<sf::Uint8> ((unsigned int)std::round(255 * car->speed() / car->target_speed()));
            rectangle.setFillColor(sf::Color(255-colorspeed,colorspeed,0,255));
            target.draw(rectangle,states);

            // this caused crash earlier
            if(car->heading_to_node!=nullptr){
                // print debug info about node placements and stuff
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
}
*/


Tests::Tests(Traffic *& traffic, sf::Mutex *& mutex) {
    m_traffic = traffic;
    m_mutex = mutex;
}

