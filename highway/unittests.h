//
// Created by Carl Schiller on 2019-01-16.
//

#include "traffic.h"
#include "SFML/Graphics.hpp"
#ifndef HIGHWAY_UNITTESTS_H
#define HIGHWAY_UNITTESTS_H

class Tests: public sf::Drawable, public sf::Transformable{
private:
    virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const;
public:
    Tests();

    static Traffic m_traffic;
    sf::Font m_font;
    void get_info(sf::Text & text, sf::Time &elapsed);

    static void placement_test();
    static void run_all_tests();
};

#endif //HIGHWAY_UNITTESTS_H
