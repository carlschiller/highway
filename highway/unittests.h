//
// Created by Carl Schiller on 2019-01-16.
//

#include "traffic.h"
#include "SFML/Graphics.hpp"
#ifndef HIGHWAY_UNITTESTS_H
#define HIGHWAY_UNITTESTS_H

class Tests{
public:
    Tests();

    Traffic m_traffic;
    void get_info(sf::Text & text, sf::Time &elapsed);

    void placement_test();
    void delete_cars_test();
    void run_one_car();
    void placement_test_2();
    void placement_test_3();
    void run_all_tests();
};

#endif //HIGHWAY_UNITTESTS_H
