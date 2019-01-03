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

Car::Car(RoadSegment *spawn_point, int lane, float vel, float target_speed, float aggressivness) {
    current_segment = spawn_point;

    current_segment->append_car(this);
    current_node = current_segment->get_node_pointer(lane);
    heading_to_node = current_node->get_next_node(lane);

    m_dist_to_next_node = Util::distance(current_node->get_x(),heading_to_node->get_x(),current_node->get_y(),heading_to_node->get_y());

    m_theta = current_node->get_theta(heading_to_node);
    m_speed = vel;
    m_target_speed = target_speed;
    m_aggressiveness = aggressivness;
    m_breaking = false;
}

void Car::update_pos(float delta_t) {
    m_dist_to_next_node -= m_speed*delta_t;
    // if we are at a new node.
    if(m_dist_to_next_node < 0){
        current_segment->remove_car(this); // remove car from this segment
        current_segment = heading_to_node->get_parent_segment(); // set new segment
        if(current_segment != nullptr){
            current_segment->append_car(this); // add car to new segment
        }
        current_node = heading_to_node; // set new current node as previous one.

        //TODO: place logic for choosing next node
        std::vector<RoadNode*> connections = current_node->get_connections();

        if(!connections.empty()){
            heading_to_node = connections[connections.size()-1];
            m_dist_to_next_node += Util::distance(current_node->get_x(),heading_to_node->get_x(),current_node->get_y(),heading_to_node->get_y());
            m_theta = current_node->get_theta(heading_to_node);
        }
    }
}


void Car::accelerate(float elapsed){
    float target = m_target_speed;
    float d_vel; // proportional control.

    if(m_speed < target*0.75){
        d_vel = m_aggressiveness*elapsed;
    }
    else{
        d_vel = m_aggressiveness*(target-m_speed)*4*elapsed;
    }

    m_speed += d_vel;
}

void Car::avoid_collision(float delta_t) {
    float min_distance = 8.0f; // for car distance.
    float ideal = min_distance+min_distance*(m_speed/20.f);
    float detection_distance = 80;

    Car * closest_car = find_closest_car();
    float radius_to_car = 1000;
    float delta_speed = 0;


    if(closest_car != nullptr) {
        radius_to_car = Util::distance_to_car(this, closest_car);
        delta_speed = closest_car->speed() - this->speed();

        if (radius_to_car < ideal) {
            m_breaking = true;
        }
    }

    if(m_breaking) {
        m_speed -= std::min(std::max((ideal - radius_to_car),0.0f) * 0.5f + abs(pow(delta_speed,2.0f)), 10.0f * delta_t);
        if(radius_to_car > ideal*1.3f){
            m_breaking = false;
        }
    } else if(radius_to_car < detection_distance && delta_speed < 0){
        m_speed -= std::min(
                abs(pow(delta_speed, 2.0f)) * pow(ideal * 0.25f / radius_to_car, 2.0f) * m_aggressiveness * 0.02f,
                10.0f * delta_t);
    }
    else {
        accelerate(delta_t);
    }

    if(m_speed < 0){
        m_speed = 0;
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

Car* Car::find_closest_car() {
    const float search_radius = 100;
    RoadSegment* origin = this->current_segment;
    std::map<RoadSegment*,bool> visited;
    std::vector<RoadNode*> queue;

    for(RoadNode & node : this->current_segment->get_nodes()){
        queue.push_back(&node);
    }

    Car* answer = nullptr;
    float best_radius = 1000;

    while(!queue.empty()){
        RoadNode * next_node = queue.back();
        queue.pop_back();

        RoadSegment * next_segment = next_node->get_parent_segment();
        if(!visited[next_segment] && Util::distance(origin->get_x(),next_segment->get_x(),origin->get_y(),next_segment->get_y()) < search_radius){
            visited[next_segment] = true;

            for(Car * car : next_segment->get_car_vector()){
                if(this != car){
                    float radius = Util::distance_to_car(this,car);
                    if(Util::is_car_behind(this,car) && Util::will_car_paths_cross(this,car) && radius < best_radius){
                        best_radius = radius;
                        answer = car;
                    }
                }
            }

            // push in new nodes.
            for(RoadNode * node : next_node->get_connections()){
                queue.push_back(node);
            }
        }

    }

    return answer;
}

float Car::x_pos() {
    float x_position = heading_to_node->get_x()-m_dist_to_next_node*cos(m_theta);

    return x_position;
}

float Car::y_pos() {
    float y_position = heading_to_node->get_y()+m_dist_to_next_node*sin(m_theta);

    return y_position;
}

float & Car::speed() {
    return m_speed;
}

float & Car::target_speed() {
    return m_target_speed;
}

float & Car::theta() {
    return m_theta;
}

RoadSegment* Car::get_segment() {
    return current_segment;
}


RoadNode::RoadNode() = default;

RoadNode::~RoadNode() = default;

RoadNode::RoadNode(float x, float y, RoadSegment * segment) {
    m_x = x;
    m_y = y;
    m_is_child_of = segment;
}

void RoadNode::set_pointer(RoadNode * next_node) {
    m_connecting_nodes.push_back(next_node);
}

RoadSegment* RoadNode::get_parent_segment() {
    return m_is_child_of;
}

std::vector<RoadNode*> & RoadNode::get_connections() {
    return m_connecting_nodes;
}

float RoadNode::get_x() {
    return m_x;
}

float RoadNode::get_y() {
    return m_y;
}

float RoadNode::get_theta(RoadNode* node) {
    for(RoadNode * road_node : m_connecting_nodes){
        if(node == road_node){
            return atan2(m_y-node->m_y,node->m_x-m_x);
        }
    }
    throw std::invalid_argument("Node given is not a connecting node");
}

RoadNode* RoadNode::get_next_node(int lane) {
    return m_connecting_nodes[lane];
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

float RoadSegment::get_x() {
    return m_x;
}

float RoadSegment::get_y() {
    return m_y;
}

int RoadSegment::get_lane_number(RoadNode * node) {
    for(int i = 0; i < m_n_lanes; i++){
        if(node == &m_nodes[i]){
            return i;
        }
    }
    throw std::invalid_argument("Node is not in this segment");
}

void RoadSegment::append_car(Car * car) {
    m_cars.push_back(car);
}

void RoadSegment::remove_car(Car * car) {
    unsigned long size = m_cars.size();
    bool found = false;
    for(int i = 0; i < size; i++){
        if(car == m_cars[i]){
            m_cars.erase(m_cars.begin()+i);
            i--;
            size--;
            found = true;
        }
    }
    if(!found){
        throw std::invalid_argument("Car is not in this segment.");
    }
}

std::vector<Car*>& RoadSegment::get_car_vector() {
    return m_cars;
}

void RoadSegment::set_theta(float theta) {
    m_theta = theta;
}

void RoadSegment::calculate_and_populate_nodes() {
    // calculates placement of nodes.
    float total_length = M_LANE_WIDTH*(m_n_lanes-1);
    float current_length = -total_length/2.0f;

    for(int i = 0; i < m_n_lanes; i++){
        float x_pos = m_x+current_length*cos(m_theta+(float)M_PI*0.5f);
        float y_pos = m_y-current_length*sin(m_theta+(float)M_PI*0.5f);
        m_nodes.emplace_back(RoadNode(x_pos,y_pos,this));
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

std::vector<RoadNode>& RoadSegment::get_nodes() {
    return m_nodes;
}

RoadSegment* RoadSegment::next_segment() {
    return m_next_segment;
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

int RoadSegment::get_total_amount_of_lanes() {
    return m_n_lanes;
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

    // populate nodes.
    for (int i = 0; i < m_segments.size(); ++i) {
        // populate nodes normally.
        if(road_vector[i].size() == 4){
            m_segments[i].set_next_road_segment(&m_segments[i+1]);
            m_segments[i].calculate_theta();
            // calculate nodes based on theta.
            m_segments[i].calculate_and_populate_nodes();

        }
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
                // calculate nodes based on theta.
                m_segments[i].calculate_and_populate_nodes();

                // make this a spawn segment
                m_spawn_positions.push_back(&m_segments[i]);
            }

        }
        // else we connect one by one.
        else{
            // take previous direction and populate nodes.
            m_segments[i].set_theta(m_segments[i-1].get_theta());
            // calculate nodes based on theta.
            m_segments[i].calculate_and_populate_nodes();
        }
    }


    // connect nodes.
    for (int i = 0; i < m_segments.size(); ++i) {
        // do normal connection, ie connect all nodes.
        if(road_vector[i].size() == 4){
            m_segments[i].set_all_node_pointers_to_next_segment();
        }
        else if(road_vector[i].size() == 5){
            if(road_vector[i][4] == "false"){
                // but do not connect nodes to new ones.
            }
            else if(road_vector[i][4] == "true"){
                m_segments[i].set_all_node_pointers_to_next_segment();
            }

        }
            // else we connect one by one.
        else{
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

std::vector<RoadSegment*>& Road::spawn_positions() {
    return m_spawn_positions;
}

std::vector<RoadSegment*>& Road::despawn_positions() {
    return m_despawn_positions;
}

const std::vector<RoadSegment>& Road::segments()const {
    return m_segments;
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

// true if car paths cross
bool Util::will_car_paths_cross(Car *a, Car *b) {
    // check if cars originate at same segment and are heading to same segment

    if(a->current_segment->get_total_amount_of_lanes() == a->heading_to_node->get_parent_segment()->get_total_amount_of_lanes() &&
    b->current_segment->get_total_amount_of_lanes() == b->heading_to_node->get_parent_segment()->get_total_amount_of_lanes()){
        int a0 = a->current_segment->get_lane_number(a->current_node);
        int a1 = a->heading_to_node->get_parent_segment()->get_lane_number(a->heading_to_node);

        int b0 = b->current_segment->get_lane_number(b->current_node);
        int b1 = b->heading_to_node->get_parent_segment()->get_lane_number(b->heading_to_node);

        int lane_dist_0 = a0 - b0;
        int lane_dist_1 = a1 - b1;
        // if they originate and end at same node, they will cross. If not, they will not cross.
        if(lane_dist_0 == 0){
            return lane_dist_1 == 0;
        }
        else{
            // they cross if this product is either zero or less
            return lane_dist_0 * lane_dist_1 <= 0;
        }
    }
    // always false if we drive off highway
    else if((a->heading_to_node->get_parent_segment()->get_total_amount_of_lanes() == 2 &&
    b->heading_to_node->get_parent_segment()->get_total_amount_of_lanes() == 1 )||(
            b->heading_to_node->get_parent_segment()->get_total_amount_of_lanes() == 2 &&
            a->heading_to_node->get_parent_segment()->get_total_amount_of_lanes() == 1 )){
        return false;
    }
    else{
        return false;
    }

}

// this works only if a's heading to is b's current segment
bool Util::is_cars_in_same_lane(Car *a, Car *b) {
    return a->heading_to_node == b->current_node;
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

float Util::distance_to_car(Car * a, Car * b){
    float delta_x = a->x_pos()-b->x_pos();
    float delta_y = b->y_pos()-a->y_pos();

    return sqrt(abs(pow(delta_x,2.0f))+abs(pow(delta_y,2.0f)));
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

float Util::distance(float x1, float x2, float y1, float y2) {
    return sqrt(abs(pow(x1-x2,2.0f))+abs(pow(y1-y2,2.0f)));
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

/*
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
        delta_speed = closest_car_ahead->speed() - this->speed();

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


}

*/

Traffic::Traffic() = default;

Traffic::~Traffic() {
    for(int i = 0; i<m_cars.size(); i++){
        delete m_cars[i];
    }
    m_cars.clear();
}

const unsigned long Traffic::n_of_cars() const{
    return m_cars.size();
}

std::mt19937& Traffic::my_engine() {
    static std::mt19937 e(std::random_device{}());
    return e;
}

void Traffic::spawn_cars(double & spawn_counter, float elapsed, double & threshold) {
    spawn_counter += elapsed;
    if(spawn_counter > threshold){
        std::exponential_distribution<double> dis(2);
        std::normal_distribution<float> aggro(3.0f,0.5f);
        std::normal_distribution<float> sp(20.0,2.0);
        std::uniform_real_distribution<float> lane(0.0f,1.0f);
        std::uniform_real_distribution<float> spawn(0.0f,1.0f);

        float speed = sp(my_engine());
        float target = speed;
        threshold = dis(my_engine());
        float aggressiveness = aggro(my_engine());
        spawn_counter = 0;
        float start_lane = lane(my_engine());
        float spawn_pos = spawn(my_engine());

        std::vector<RoadSegment*> segments = m_road.spawn_positions();
        RoadSegment * seg;
        Car * new_car;
        if(spawn_pos < 0.9){
            seg = segments[0];
            if(start_lane < 0.33){
                new_car = new Car(seg,0,speed,target,aggressiveness);
            }
            else if(start_lane < 0.67){
                new_car = new Car(seg,1,speed,target,aggressiveness);
            }
            else{
                new_car = new Car(seg,2,speed,target,aggressiveness);
            }
        }
        else{
            seg = segments[1];
            new_car = new Car(seg,0,speed,target,aggressiveness);
        }

        Car * closest_car_ahead = new_car->find_closest_car();

        if(closest_car_ahead == nullptr && closest_car_ahead != new_car){
            m_cars.push_back(new_car);
        }
        else{
            float dist = Util::distance_to_car(new_car,closest_car_ahead);
            if(dist < 10){
                seg->remove_car(new_car);
                delete new_car;
                new_car = nullptr;
            }
            else if (dist < 40){
                new_car->speed() = closest_car_ahead->speed();
                m_cars.push_back(new_car);
            }
            else{
                m_cars.push_back(new_car);
            }
        }
    }
}

void Traffic::despawn_cars() {
    int car_amount = static_cast<int>(m_cars.size());
    for(int i = 0; i < car_amount; i++){
        for(RoadSegment * seg : m_road.despawn_positions()){
            if(m_cars[i] != nullptr){
                if(m_cars[i]->get_segment() == seg){
                    m_cars[i]->current_segment->remove_car(m_cars[i]); // remove this pointer shit
                    delete m_cars[i]; // delete pointer
                    m_cars[i] = nullptr; // set pointer to nullptr
                }
            }
        }
    }
    m_cars.erase(std::remove(m_cars.begin(),m_cars.end(),nullptr),m_cars.end()); // safe remove all nullptrs
}

void Traffic::update(float elapsed_time) {
    for(Car * car : m_cars){
        car->avoid_collision(elapsed_time);
    }
    for(Car * car : m_cars){
        car->update_pos(elapsed_time);
    }
}

const std::vector<Car*> &Traffic::get_cars()const {
    return m_cars;
}

float Traffic::get_avg_flow() {
    float flow = 0;
    for(Car * car : m_cars){
        flow += car->speed()/car->target_speed();
    }
    return flow/(float)n_of_cars();
}

const Road & Traffic::road() const {
    return m_road;
}