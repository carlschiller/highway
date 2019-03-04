//
// Created by Carl Schiller on 2019-03-04.
//

#ifndef HIGHWAY_UTIL_H
#define HIGHWAY_UTIL_H

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// Util                                                                 //
//                                                                      //
// Help functions for Car class.                                        //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "car.h"

class Util{
public:
    static std::vector<std::string> split_string_by_delimiter(const std::string & str, const char delim);
    static bool is_car_behind(Car * a, Car * b);
    static bool will_car_paths_cross(Car *a, Car*b);
    static bool is_cars_in_same_lane(Car*a,Car*b);
    static bool merge_helper(Car*a, int merge_to_lane);
    static float distance_to_line(float theta,  float x,  float y);
    static float distance_to_proj_point(float theta,  float x,  float y);
    static float distance_to_car(Car * a, Car * b);
    static Car * find_closest_radius(std::vector<Car> &cars, float x, float y);
    static float get_min_angle(float ang1, float ang2);
    static float distance(float x1, float x2, float y1, float y2);
};

#endif //HIGHWAY_UTIL_H
