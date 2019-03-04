cmake_minimum_required(VERSION 3.2)
project(highway)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -g -Ofast")

set(SOURCE_FILES cppfiles/main.cpp cppfiles/traffic.cpp headers/traffic.h cppfiles/simulation.cpp headers/simulation.h cppfiles/unittests.cpp headers/unittests.h cppfiles/car.cpp headers/car.h cppfiles/roadnode.cpp headers/roadnode.h cppfiles/roadsegment.cpp headers/roadsegment.h cppfiles/roadsegment.cpp headers/roadsegment.h cppfiles/util.cpp headers/util.h cppfiles/road.cpp headers/road.h)
add_executable(highway ${SOURCE_FILES})

set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules/")

find_package(SFML REQUIRED system window graphics network audio)
if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(highway ${SFML_LIBRARIES})
endif()