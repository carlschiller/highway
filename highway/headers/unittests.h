//
// Created by Carl Schiller on 2019-03-01.
//


#ifndef HIGHWAY_UNITTESTS_H
#define HIGHWAY_UNITTESTS_H

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// Tests                                                                //
//                                                                      //
// Testing the various functions.                                       //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "traffic.h"
#include "SFML/Graphics.hpp"

class Tests{
private:
    Traffic * m_traffic;
    sf::Mutex * m_mutex;
    void placement_test();
    void delete_cars_test();
    void run_one_car();
    void placement_test_2();
    void placement_test_3();
public:
    Tests() = delete;
    Tests(Traffic *& traffic, sf::Mutex *& mutex);

    void run_all_tests();
};

#endif //HIGHWAY_UNITTESTS_H
