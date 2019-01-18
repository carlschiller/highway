//
// Created by Carl Schiller on 2018-12-19.
//

#include <vector>
#include "SFML/Graphics.hpp"
#include "traffic.h"

#ifndef HIGHWAY_WINDOW_H
#define HIGHWAY_WINDOW_H

class Simulation : public sf::Drawable, public sf::Transformable{
public:
    Simulation();
    explicit Simulation(bool debug,int sim_speed);

    void update(sf::Time elapsed, double & spawn_counter, double & threshold);
    float get_flow();
    //void car_debug(sf::Time t0);
    void get_info(sf::Text & text, sf::Time &elapsed);
private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
private:
    Traffic m_traffic;
    sf::Texture m_texture;
    bool m_debug;
    int m_sim_speed;
    sf::Font m_font;
};


#endif //HIGHWAY_WINDOW_H
