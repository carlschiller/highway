//
// Created by Carl Schiller on 2018-12-19.
//

#include "traffic.h"
#include <cmath>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <random>
#include <vector>

float get_min_angle(const float ang1, const float ang2){
    float abs_diff = abs(ang1-ang2);
    float score = std::min(2.0f*(float)M_PI-abs_diff,abs_diff);
    return score;
}

Car::Car() = default;

Car::Car(float x_pos, float y_pos, float vel, float theta, float target_speed, float aggressiveness) {
    // volvo v70 dimensions.
    m_length = 4.7;
    m_width = 1.7;

    m_x_pos = x_pos;
    m_y_pos = y_pos;
    m_vel = vel;
    m_theta = theta;

    // target speed and how fast to reach speed.
    m_target_speed = target_speed;
    m_aggressiveness = aggressiveness;
}

void Car::update_pos(float delta_t) {
    m_x_pos += cos(m_theta)*m_vel*delta_t;
    m_y_pos += -sin(m_theta)*m_vel*delta_t;
}

void Car::accelerate(float delta_theta){
    float target = m_target_speed*(1-abs(delta_theta)/(2.0f*3.141f));

    float d_vel; // proportional control.
    if(m_vel < target*0.75){
        d_vel = m_aggressiveness;
    }
    else{
        d_vel = m_aggressiveness*(target-m_vel)*4;
    }
    if(m_vel + d_vel > 0){
        m_vel += d_vel;
    }
}

void Car::steer(float new_theta) {
    m_theta = (new_theta);
}

float & Car::x_pos() {
    return m_x_pos;
}

float & Car::y_pos() {
    return m_y_pos;
}

float & Car::vel() {
    return m_vel;
}

float & Car::target_speed() {
    return m_target_speed;
}

float & Car::length() { return m_length; }

float & Car::width() { return m_width; }

float & Car::theta() { return m_theta; }

void Car::avoid_collision(std::vector<Car> &cars, int i,sf::Time elapsed) {
    for(int j = 0; j < cars.size(); j++){
        float radius_to_next_car,theta_to_car,
                theta_diff_to_car_position,theta_diff_between_car_directions,
                delta_speed, three_second;

        const float min_distance_to = 10.0f;
        three_second = abs(m_vel*3.0f);
        if(i!=j){
            radius_to_next_car = sqrt(pow(cars[j].x_pos()-m_x_pos,2.0f)
                    +pow(cars[j].y_pos()-m_y_pos,2.0f));


            theta_to_car = atan2(-cars[j].m_y_pos+m_y_pos,-cars[j].x_pos()+m_x_pos);
            theta_diff_to_car_position = get_min_angle(theta_to_car,m_theta);
            theta_diff_between_car_directions = get_min_angle(m_theta,cars[j].m_theta);
            delta_speed = (m_vel-cars[j].m_vel);

            if(theta_diff_to_car_position < M_PI*0.3 &&
               theta_diff_between_car_directions < M_PI*0.30 && delta_speed < 0){
                if(radius_to_next_car < three_second){
                    m_vel -= delta_speed*m_aggressiveness*pow(1-radius_to_next_car/three_second,2.0f);
                    if(m_vel < 0){
                        m_vel = 0;
                    }
                }
                if(radius_to_next_car < min_distance_to){

                }
            }
        }
    }
}

std::vector<std::vector<int>> load_from_text(const std::string & name){
    std::vector<std::vector<int>> vec;
    std::ifstream stream(name);
    char delim = ',';
    std::string line;

    while(std::getline(stream,line)){
        std::vector<int> subvec;
        std::string character;
        std::stringstream str(line);
        while(std::getline(str,character,delim)){
            subvec.push_back(std::stoi(character));
        }

        vec.push_back(subvec);
    }

    return vec;
}

std::map<Spawn_positions,std::vector<float>> load_positions(){
    std::map<Spawn_positions ,std::vector<float>> my_map;
    std::vector<float> pos1 = {82,599,1.36f};
    my_map[Spawn_positions::LOWER_LEFT] = pos1;
    return my_map;
};

Traffic::Traffic() = default;

Traffic::Traffic(std::string filename) {
    m_allowed_zone = load_from_text(filename);
    m_spawn_positions = load_positions();
}

unsigned long Traffic::n_of_cars() {
    return m_cars.size();
}

std::mt19937& Traffic::my_engine() {
    static std::mt19937 e(std::random_device{}());
    return e;
}

void Traffic::spawn_cars(double & spawn_counter, sf::Time & elapsed, double & threshold) {
    spawn_counter += elapsed.asSeconds();
    if(spawn_counter > threshold + 1){
        std::exponential_distribution<double> dis(1.0);
        std::normal_distribution<float> sp(20.0,3.0);

        float speed = sp(my_engine());
        threshold = dis(my_engine());

        spawn_counter = 0;
        std::vector<float> start_pos = m_spawn_positions[Spawn_positions::LOWER_LEFT];
        m_cars.emplace_back(Car(start_pos[0],start_pos[1],speed,start_pos[2],speed,0.05f));
    }
}

float Traffic::get_theta(float xpos, float ypos, float speed, float current_theta) {
    std::vector<float> theta_candidates;
    float radius;
    if(speed > 8.0){
        radius = speed/4.0f;
    }
    else{
        radius = 2.0f;
    }

    const int divisions = 60;

    for(int i = 0; i < divisions; i++){
        float angle = (float)(i)/(float)divisions*2.0f*3.141f;
        auto x_temp = (int)round(xpos + radius*cos(angle));
        auto y_temp = (int)round(ypos - radius*sin(angle));
        if( y_temp < m_allowed_zone.size() && y_temp >= 0 &&
            x_temp < m_allowed_zone[0].size() && x_temp >= 0 ){
            if(m_allowed_zone[y_temp][x_temp] == 1) {
                theta_candidates.push_back(angle);
            }
        }
    }

    if(theta_candidates.empty()){
        return current_theta;
    }
    else{
        float best_score = 100000;
        float best_theta = 100000;
        for(float c : theta_candidates){
            float score = get_min_angle(c,current_theta);
            if( score < best_score){
                best_score = score;
                best_theta = c;
            }
        }
        return best_theta;
    }
}

void Traffic::update_position(int i, sf::Time & elapsed_time) {
    // look in a circle speed/4 m around car to find next angle to drive in.
    Car & car = m_cars[i];
    float old_theta = car.theta();
    float theta = get_theta(car.x_pos(),car.y_pos(),car.vel(),car.theta());
    car.steer(theta);
    car.accelerate(theta-old_theta);

    car.avoid_collision(m_cars,i,elapsed_time);

    car.update_pos(elapsed_time.asSeconds());
}

void Traffic::update(sf::Time & elapsed_time) {
    for(int i = 0; i < m_cars.size(); i++){
        update_position(i,elapsed_time);
    }
}

void Traffic::debug(sf::Time t0) {
    if(n_of_cars() > 0){
        std::string message;
        message += "Vel: " + std::to_string(m_cars[0].vel()*3.6f).substr(0,4) + " km/h, time: " +
                   std::to_string(t0.asSeconds()).substr(0,3) + " s, theta:" + std::to_string(m_cars[0].theta()).substr(0,4) +
                " ,x:" + std::to_string(m_cars[0].x_pos()).substr(0,3) + " ,y:" + std::to_string(m_cars[0].y_pos()).substr(0,3);
        std::cout << message << std::endl;
    }
}


void Traffic::force_spawn_car() {
    m_cars.emplace_back(Car(82,599,20.0,13,20,0.05));
}

const std::vector<Car> &Traffic::get_cars()const {
    return m_cars;
}
