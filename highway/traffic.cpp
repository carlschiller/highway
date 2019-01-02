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

Car::Car() = default;

Car::Car(float x_pos, float y_pos, float vel, float theta, float target_speed, float aggressiveness) {
    // volvo v70 dimensions.
    m_length = 4.7;
    m_width = 1.7;

    m_x_pos = x_pos;
    m_y_pos = y_pos;
    m_vel = vel;
    m_theta = theta;

    lane_switch = false;
    m_breaking = false;

    // target speed and how fast to reach speed.
    m_target_speed = target_speed;
    m_aggressiveness = aggressiveness;
}

void Car::update_pos(float delta_t) {
    m_x_pos += cos(m_theta)*m_vel*delta_t;
    m_y_pos += -sin(m_theta)*m_vel*delta_t;
}

void Car::accelerate(float delta_theta, Car * ahead){
    float target = m_target_speed*(1-abs(delta_theta)/(2.0f*3.141f));
    float d_vel; // proportional control.

    if(m_vel < target*0.75){
        d_vel = m_aggressiveness;
    }
    else{
        d_vel = m_aggressiveness*(target-m_vel)*4;
    }

    m_vel += d_vel;
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


RoadNode::RoadNode() = default;

RoadNode::~RoadNode() = default;

RoadNode::RoadNode(float x, float y) {
    m_x = x;
    m_y = y;
}

void RoadNode::set_pointer(RoadNode * next_node) {
    m_connecting_nodes.push_back(next_node);
}

float RoadNode::get_theta(int node_number) {
    return atan2(m_y-m_connecting_nodes[node_number]->m_y,m_connecting_nodes[node_number]->m_x-m_x);
}


RoadSegment::RoadSegment() = default;

RoadSegment::~RoadSegment() = default;

RoadSegment::RoadSegment(float x, float y, RoadSegment * next_segment, int lanes) {
    m_x = x;
    m_y = y;

    m_next_segment = next_segment;

    m_theta = atan2(m_y-m_next_segment->m_y,m_next_segment->m_x-m_x);

    m_n_lanes = lanes;
    m_nodes.reserve(lanes);

    calculate_and_populate_nodes();
}

RoadSegment::RoadSegment(float x, float y, float theta, int lanes) {
    m_x = x;
    m_y = y;

    m_next_segment = nullptr;

    m_theta = theta;

    m_n_lanes = lanes;
    m_nodes.reserve(lanes);

    calculate_and_populate_nodes();
}

RoadSegment::RoadSegment(float x, float y, int lanes) {
    m_x = x;
    m_y = y;

    m_next_segment = nullptr;

    m_n_lanes = lanes;
    m_nodes.reserve(m_n_lanes);

    // can't set nodes if we don't have a theta.
}

float RoadSegment::get_theta() {
    return m_theta;
}

void RoadSegment::set_theta(float theta) {
    m_theta = theta;
}

void RoadSegment::calculate_and_populate_nodes() {
    // calculates placement of nodes.
    float total_length = M_LANE_WIDTH*(m_n_lanes-1);
    float current_length = -total_length/2.0f;

    for(int i = 0; i < m_n_lanes; i++){
        float x_pos = m_x+current_length*cos(m_theta);
        float y_pos = m_y-current_length*sin(m_theta);
        m_nodes.emplace_back(RoadNode(x_pos,y_pos));
        current_length += M_LANE_WIDTH;
    }
}

void RoadSegment::set_next_road_segment(RoadSegment * next_segment) {
    m_next_segment = next_segment;
}

void RoadSegment::calculate_theta() {
    m_theta = atan2(m_y-m_next_segment->m_y,m_next_segment->m_x-m_x);
}

RoadNode* RoadSegment::get_node_pointer(int n) {
    return &m_nodes[n];
}

void RoadSegment::set_all_node_pointers_to_next_segment() {
    for(RoadNode & node: m_nodes){
        for(int i = 0; i < m_next_segment->m_n_lanes; i++){
            node.set_pointer(m_next_segment->get_node_pointer(i));
        }
    }
}

void RoadSegment::set_node_pointer_to_node(int from_node_n, int to_node_n, RoadSegment *next_segment) {
    RoadNode * pointy = next_segment->get_node_pointer(to_node_n);

    m_nodes[from_node_n].set_pointer(pointy);
}

Road::Road() {
    if(!load_road()){
       std::cout << "Error in loading road.\n";
    };
}

Road::~Road() = default;

void Road::insert_segment(RoadSegment & segment) {
    m_segments.push_back(segment);
}

bool Road::load_road() {
    bool loading = true;
    std::ifstream stream;
    stream.open(M_FILENAME);

    std::vector<std::vector<std::string>> road_vector;
    road_vector.reserve(100);

    if(stream.is_open()){
        std::string line;
        std::vector<std::string> tokens;
        while(std::getline(stream,line)){
            tokens = Util::split_string_by_delimiter(line,' ');
            if(tokens[0] != "#"){
                road_vector.push_back(tokens);
            }
        }
    }
    else{
        loading = false;
    }


    // load segments into memory.
    for(std::vector<std::string> & vec : road_vector){
        m_segments.emplace_back(RoadSegment(std::stof(vec[1]),std::stof(vec[2]),std::stoi(vec[3])));
    }

    // connect nodes.
    for (int i = 0; i < m_segments.size(); ++i) {
        // do normal connection, ie connect all nodes.
        if(road_vector[i].size() == 4){
            m_segments[i].set_next_road_segment(&m_segments[i+1]);
            m_segments[i].calculate_theta();
            m_segments[i].set_all_node_pointers_to_next_segment();
        }
        // else we do not connect to new.
        else if(road_vector[i].size() == 5){
            if(road_vector[i][4] == "false"){
                // take previous direction and populate nodes.
                m_segments[i].set_theta(m_segments[i-1].get_theta());
                m_segments[i].calculate_and_populate_nodes();
                // but do not connect nodes to new ones.

                // make this a despawn segment
                m_despawn_positions.push_back(&m_segments[i]);
            }
            else if(road_vector[i][4] == "true"){
                m_segments[i].set_next_road_segment(&m_segments[i+1]);
                m_segments[i].calculate_theta();
                m_segments[i].set_all_node_pointers_to_next_segment();

                // make this a spawn segment
                m_spawn_positions.push_back(&m_segments[i]);
            }

        }
        // else we connect one by one.
        else{
            // take previous direction and populate nodes.
            m_segments[i].set_theta(m_segments[i-1].get_theta());
            m_segments[i].calculate_and_populate_nodes();

            // manually connect nodes.
            int amount_of_pointers = (int)road_vector[i].size()-4;
            for(int j = 0; j < amount_of_pointers/3; j++){
                int current_pos = 4+j*3;
                RoadSegment * next_segment = &m_segments[std::stoi(road_vector[i][current_pos+2])];
                m_segments[i].set_node_pointer_to_node(std::stoi(road_vector[i][current_pos]),std::stoi(road_vector[i][current_pos+1]),next_segment);
            }
        }
    }

    return loading;
}

std::vector<std::string> Util::split_string_by_delimiter(const std::string &str, const char delim) {
    std::stringstream ss(str);
    std::string item;
    std::vector<std::string> answer;
    while(std::getline(ss,item,delim)){
        answer.push_back(item);
    }
    return answer;
}

// if a is behind of b, return true. else false
bool Util::is_car_behind(Car * a, Car * b){
    if(a!=b){
        float theta_to_car_b = atan2(a->y_pos()-b->y_pos(),b->x_pos()-a->x_pos());
        float theta_difference = get_min_angle(a->theta(),theta_to_car_b);
        return theta_difference < M_PI*0.45;
    }
    else{
        return false;
    }

}

float Util::distance_to_line(const float theta, const float x, const float y){
    float x_hat,y_hat;
    x_hat = cos(theta);
    y_hat = -sin(theta);

    float proj_x = (x*x_hat+y*y_hat)*x_hat;
    float proj_y = (x*x_hat+y*y_hat)*y_hat;
    float dist = sqrt(abs(pow(x-proj_x,2.0f))+abs(pow(y-proj_y,2.0f)));

    return dist;
}

float Util::distance_to_proj_point(const float theta, const float x, const float y){
    float x_hat,y_hat;
    x_hat = cos(theta);
    y_hat = -sin(theta);
    float proj_x = (x*x_hat+y*y_hat)*x_hat;
    float proj_y = (x*x_hat+y*y_hat)*y_hat;
    float dist = sqrt(abs(pow(proj_x,2.0f))+abs(pow(proj_y,2.0f)));

    return dist;
}

float Util::distance_to_car(Car & a, Car & b){
    float delta_x = a.x_pos()-b.x_pos();
    float delta_y = b.y_pos()-a.y_pos();

    return sqrt(abs(pow(delta_x,2.0f))+abs(pow(delta_y,2.0f)));
}


bool Util::find_connected_path(Car & ref, Car & car, std::vector<std::vector<int>> & allowed_zone, const int buffer){
    auto init_x = (int)std::round(ref.x_pos());
    auto init_y = (int)std::round(ref.y_pos());
    auto target_x = (int)std::round(car.x_pos());
    auto target_y =  (int)std::round(car.y_pos());

    auto search_radius = (unsigned int)ceil(std::max(abs(init_x-target_x),abs(target_y-init_y)))+buffer;
    unsigned int search_diameter = 2*search_radius+1;

    std::vector<std::vector<bool>> visited(search_diameter,std::vector<bool>(search_diameter,false));
    bool connected = false;

    std::vector<std::vector<int>> next_square;
    int current_x = init_x;
    int current_y = init_y;
    std::vector<int> current_square = {current_x,current_y};
    next_square.push_back(current_square);

    while(!next_square.empty()){
        current_square = next_square.back();
        next_square.pop_back();

        if(current_square[0] == target_x && current_square[1] == target_y){
            connected = true;
            break;
        }

        if(current_square[0] >= 0 && current_square[0] < allowed_zone[0].size() && current_square[1] >= 0 && current_square[1] < allowed_zone.size()){
            if(allowed_zone[current_square[1]][current_square[0]] == 1){
                if(abs(current_square[1]-init_y) <= search_radius &&abs(current_square[0]-init_x) <= search_radius){
                    if(!visited[current_square[1]-init_y+search_radius][current_square[0]-init_x+search_radius]){

                        visited[current_square[1]-init_y+search_radius][current_square[0]-init_x+search_radius] = true;

                        std::vector<int> new_square = current_square;

                        new_square[0]++;
                        next_square.push_back(new_square);
                        new_square[0]--;

                        new_square[0]--;
                        next_square.push_back(new_square);
                        new_square[0]++;

                        new_square[1]++;
                        next_square.push_back(new_square);
                        new_square[1]--;

                        new_square[1]--;
                        next_square.push_back(new_square);
                    }
                }
            }
        }
    }

    return connected;
}

Car * Util::find_closest_car(std::vector<Car> &cars, Car * ref, std::vector<std::vector<int>> & allowed_zone){
    Car * answer = nullptr;
    float search_radius = 100;
    int buffer = 10;

    std::map<float,Car*> candidates;

    // calculate distances
    for(Car & car : cars){
        if(ref!=&car){
            float dist = distance_to_car(*ref,car);
            if(is_car_behind(ref,&car) && dist < search_radius){
                candidates[dist] = &car;
            }
        }
    }

    // loop through by smallest distance and check if it is connected.
    for(auto it : candidates){
        if(find_connected_path(*ref,*it.second,allowed_zone,buffer)){
            answer = it.second;
            break;
        }
    }

    return answer;
}

Car * Util::find_closest_radius(std::vector<Car> &cars, const float x, const float y){
    Car * answer = nullptr;

    float score = 100000;
    for(Car & car : cars){
        float distance = sqrt(abs(pow(car.x_pos()-x,2.0f))+abs(pow(car.y_pos()-y,2.0f)));
        if(distance < score){
            score = distance;
            answer = &car;
        }
    }

    return answer;
}

float Util::get_min_angle(const float ang1, const float ang2){
    float abs_diff = abs(ang1-ang2);
    float score = std::min(2.0f*(float)M_PI-abs_diff,abs_diff);
    return score;
}

/*
Car * find_car_to_side(std::vector<Car> &cars, int i, Car & ref_car, float min_radius ,float view_angle){
    Car * answer = nullptr;

    std::vector<Car*> candidates;
    candidates.reserve(cars.size());

    float radius_to_next_car, theta_to_car, theta_diff_to_car_position,
            theta_diff_between_car_directions;

    float best_radius = min_radius;
    for(int j = 0; j < cars.size(); j++){
        if(i!=j){
            radius_to_next_car = sqrt(abs(pow(cars[j].x_pos()-ref_car.x_pos(),2.0f))
                                      +abs(pow(cars[j].y_pos()-ref_car.y_pos(),2.0f)));
            theta_to_car = atan2(-cars[j].y_pos()+ref_car.y_pos(),cars[j].x_pos()-ref_car.x_pos());

            theta_diff_to_car_position = get_min_angle(theta_to_car,ref_car.theta());
            theta_diff_between_car_directions = get_min_angle(ref_car.theta(),cars[j].theta());


            if(abs(theta_diff_to_car_position) > view_angle && abs(theta_diff_to_car_position) < M_PI*0.5 &&
               abs(theta_diff_between_car_directions) < M_PI*0.1 && radius_to_next_car < best_radius){
                best_radius = radius_to_next_car;
                answer = &cars[j];
            }
        }
    }
    return answer;
}
*/

void Car::avoid_collision(std::vector<Car> &cars, int i,float & elapsed, float delta_theta,
                          std::vector<std::vector<int>> & allowed_zone) {
    float min_distance = 8.0f; // for car distance.
    float ideal = min_distance+min_distance*(m_vel/20.f);
    float detection_distance = m_vel*4.0f;

    Car * closest_car_ahead = Util::find_closest_car(cars,this,allowed_zone);

    float delta_speed = 0;
    float radius_to_car = 200;

    if(closest_car_ahead != nullptr) {
        radius_to_car = Util::distance_to_car(*this, *closest_car_ahead);
        delta_speed = closest_car_ahead->vel() - this->vel();

        if (radius_to_car < ideal) {
            m_breaking = true;
        }

    }

    if(m_breaking) {
        m_vel -= std::min(std::max((ideal - radius_to_car),0.0f) * 0.5f + abs(pow(delta_speed,2.0f)), 10.0f * elapsed);
        if(radius_to_car > ideal*1.3f){
            m_breaking = false;
        }
    } else if(radius_to_car < detection_distance && delta_speed < 0){
        m_vel -= std::min(
                abs(pow(delta_speed, 2.0f)) * pow(ideal * 0.25f / radius_to_car, 2.0f) * m_aggressiveness * 2,
                10.0f * elapsed);
    }
    else {
        accelerate(delta_theta,closest_car_ahead);
    }

    if(m_vel < 0){
        m_vel = 0;
    }
    /*
    else{
        m_vel -= std::min(abs(delta_speed)*ideal/radius_to_car + abs(pow(delta_speed,2.0f))*0.25f , 10.0f*elapsed);
    }
    else if () {
        m_vel -= std::min(
                abs(pow(delta_speed, 2.0f)) * pow(ideal * 0.5f / radius_to_car, 2.0f) * m_aggressiveness * 2,
                10.0f * elapsed);
        } else{
            accelerate(delta_theta,closest_car_ahead);
        }
    else {

    }
    */

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

std::map<Spawn_positions,std::vector<float>> load_spawn_points(){
    std::map<Spawn_positions ,std::vector<float>> my_map;
    std::vector<float> pos1 = {82,599,1.36f};
    std::vector<float> pos2 = {377,309,1.88f};
    my_map[Spawn_positions::LOWER_LEFT] = pos1;
    my_map[Spawn_positions::RAMP] = pos2;
    return my_map;
};

std::map<Despawn_positions,std::vector<float>> load_despawn_points(){
    std::map<Despawn_positions ,std::vector<float>> my_map;
    std::vector<float> pos1 = {546,21,1.88f};
    std::vector<float> pos2 = {344,274,1.88f};
    my_map[Despawn_positions::UPPER_RIGHT] = pos1;
    my_map[Despawn_positions::RAMP] = pos2;
    return my_map;
};

std::map<Lane_positions ,std::vector<float>> load_lane_points(){
    std::map<Lane_positions ,std::vector<float>> my_map;
    std::vector<float> lower_left = {99,547,(float)M_PI*0.5f*0.8f,(float)M_PI*0.5f*0.6f};

    my_map[Lane_positions ::LOWER_LEFT] = lower_left;
    return my_map;
};

Traffic::Traffic() = default;

Traffic::Traffic(std::string filename) {
    m_allowed_zone = load_from_text(filename);
    m_spawn_positions = load_spawn_points();
    m_despawn_positions = load_despawn_points();
    m_lane_switch_points = load_lane_points();
}

const unsigned long Traffic::n_of_cars() const{
    return m_cars.size();
}

std::mt19937& Traffic::my_engine() {
    static std::mt19937 e(std::random_device{}());
    return e;
}

void Traffic::spawn_cars(double & spawn_counter, sf::Time & elapsed, double & threshold, float sim_speed) {
    spawn_counter += elapsed.asSeconds()*sim_speed;
    if(spawn_counter > threshold){
        std::exponential_distribution<double> dis(0.5);
        std::normal_distribution<float> aggro(0.05f,0.01f);
        std::normal_distribution<float> sp(20.0,2.0);
        std::uniform_real_distribution<float> ramp(0.0f,1.0f);

        float speed = sp(my_engine());
        float target = speed;
        threshold = dis(my_engine());
        float aggressiveness = aggro(my_engine());
        float ramp_spawn = ramp(my_engine());
        Spawn_positions pos;

        spawn_counter = 0;
        if(ramp_spawn > 0.95){
            pos = Spawn_positions ::RAMP;
            speed = speed*0.25f;
        }
        else{
            pos = Spawn_positions ::LOWER_LEFT;
        }

        std::vector<float> start_pos = m_spawn_positions[pos];
        Car new_car = Car(start_pos[0],start_pos[1],speed,start_pos[2],target,aggressiveness);

        // look if car is near spawn point
        Car * closest_car = Util::find_closest_radius(m_cars,start_pos[0],start_pos[1]);
        if(closest_car != nullptr){
            float distance = Util::distance_to_car(*closest_car,new_car);
            if(distance < 50 && distance > 10){
                new_car.vel() = closest_car->vel();
                m_cars.push_back(new_car);
            }
            else if(distance > 50){
                m_cars.push_back(new_car);
            }
        }
        else{
            m_cars.push_back(new_car);
        }
    }
}

void Traffic::despawn_cars() {
    for(const auto & it : m_despawn_positions){
        unsigned long size = m_cars.size();

        for(int i =0; i< size; i++){
            float despawn_radius = sqrt(abs(pow(m_cars[i].x_pos()-it.second[0],2.0f))+abs(pow(m_cars[i].y_pos()-it.second[1],2.0f)));
            if(despawn_radius < 2){
                m_cars.erase(m_cars.begin()+i);
                size--;
                i--;

            }
        }
    }
}

float Traffic::get_theta(float xpos, float ypos, float speed, float current_theta, bool & lane_switch) {
    std::vector<float> theta_candidates;
    float radius = 2.0f;

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
            float score = Util::get_min_angle(c,current_theta);
            if( score < best_score){
                best_score = score;
                best_theta = c;
            }
        }

        for(const auto & it : m_lane_switch_points){
            float rad = sqrt(abs(pow(xpos-it.second[0],2.0f))+abs(pow(ypos-it.second[1],2.0f)));
            if(!lane_switch && rad < 2){
                std::uniform_real_distribution<float> prob(0.0f,1.0f);
                float coin_flip = prob(my_engine());
                if(coin_flip > 1.0){
                    best_theta = it.second[3];
                }
                lane_switch = true;
            }
            else if (rad > 2){
                lane_switch = false;
            }
        }

        return best_theta;
    }
}

void Traffic::update_speed(int i, float & elapsed_time) {
    // look in a circle speed/4 m around car to find next angle to drive in.
    Car & car = m_cars[i];
    float old_theta = car.theta();
    float theta = get_theta(car.x_pos(),car.y_pos(),car.vel(),car.theta(),car.lane_switch);
    car.steer(theta);
    car.avoid_collision(m_cars,i,elapsed_time,theta-old_theta,m_allowed_zone);
}

void Traffic::update(sf::Time & elapsed_time, float sim_speed) {
    float elapsed = elapsed_time.asSeconds()*sim_speed;
    for(int i = 0; i < m_cars.size(); i++){
        update_speed(i,elapsed);
    }
    for(Car & car : m_cars){
        car.update_pos(elapsed);
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

float Traffic::get_avg_flow() {
    float flow = 0;
    for(Car & car : m_cars){
        flow += car.vel()/car.target_speed();
    }
    return flow/(float)n_of_cars();
}
