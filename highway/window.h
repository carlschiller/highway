//
// Created by Carl Schiller on 2018-12-19.
//

#include <vector>
#include "SFML/Graphics.hpp"
#include "traffic.h"

#ifndef HIGHWAY_WINDOW_H
#define HIGHWAY_WINDOW_H

class Car_squares : public sf::Drawable, public sf::Transformable{
public:
    Car_squares();

    void update(sf::Time elapsed);
    void car_debug(sf::Time t0);
private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
    sf::VertexArray m_vertices;
    std::vector<Car> m_cars;
    sf::Texture m_texture;
};

#endif //HIGHWAY_WINDOW_H
