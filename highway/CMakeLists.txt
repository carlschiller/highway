cmake_minimum_required(VERSION 3.2)
project(highway)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -g -Ofast")

set(SOURCE_FILES main.cpp traffic.cpp traffic.h window.cpp window.h unittests.cpp unittests.h)
add_executable(highway ${SOURCE_FILES})

set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules/")

find_package(SFML REQUIRED system window graphics network audio)
if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(highway ${SFML_LIBRARIES})
endif()