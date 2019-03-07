//
// Created by Carl Schiller on 2018-12-19.
//

#include <iostream>
#include "../headers/traffic.h"
#include "../headers/car.h"
#include "../headers/road.h"
#include "../headers/util.h"

////////////////////////////////////////////////////////////////////////////////
/// Constructor.

/*
Traffic::Traffic() {
    debug = false;
    if(!m_font.loadFromFile("/Library/Fonts/Andale mono.ttf")){

    }
}
*/

////////////////////////////////////////////////////////////////////////////////
/// Constructor with debug bool, if we want to use debugging information.

Traffic::Traffic(std::vector<bool> bargs, std::vector<float> args) :
    debug(bargs[0]),
    m_aggro(args[0]),
    m_aggro_sigma(args[1]),
    m_spawn_freq(args[2]),
    m_speed(args[3]),
    
    m_lane_0_spawn_prob(args[4]),
    m_lane_1_spawn_prob(args[5]),
    m_lane_2_spawn_prob(args[6]),
    m_ramp_0_spawn_prob(args[7]),
    
    m_min_dist_to_car_in_front(args[8]),
    m_min_overtake_dist_trigger(args[9]),
    m_max_overtake_dist_trigger(args[10]),
    m_overtake_done_dist(args[11]),
    m_merge_min_dist(args[12]),
    m_search_radius_around(args[13]),
    m_search_radius_to_car_in_front(args[14]),
    m_ramp_meter_period(args[17]),
    m_ramp_meter(bargs[1]),
    m_multiplier(args[15])
{
    probs.push_back(m_lane_0_spawn_prob);
    probs.push_back(m_lane_1_spawn_prob);
    probs.push_back(m_lane_2_spawn_prob);
    probs.push_back(m_ramp_0_spawn_prob);

    if(!m_font.loadFromFile("/Library/Fonts/Andale mono.ttf")){

    }

    Road::shared().ramp_meter_position->ramp_counter = 0;
    Road::shared().ramp_meter_position->meter = m_ramp_meter;
    Road::shared().ramp_meter_position->period = m_ramp_meter_period;
}

////////////////////////////////////////////////////////////////////////////////
/// Copy constructor, deep copies all content.

Traffic::Traffic(const Traffic &ref) :
    debug(ref.debug),
    m_font(ref.m_font),
    m_aggro(ref.m_aggro),
    m_aggro_sigma(ref.m_aggro_sigma),
    m_spawn_freq(ref.m_spawn_freq),
    m_speed(ref.m_speed),
    m_lane_0_spawn_prob(ref.m_lane_0_spawn_prob),
    m_lane_1_spawn_prob(ref.m_lane_1_spawn_prob),
    m_lane_2_spawn_prob(ref.m_lane_2_spawn_prob),
    m_ramp_0_spawn_prob(ref.m_ramp_0_spawn_prob),
    m_min_dist_to_car_in_front(ref.m_min_dist_to_car_in_front),
    m_min_overtake_dist_trigger(ref.m_min_overtake_dist_trigger),
    m_max_overtake_dist_trigger(ref.m_max_overtake_dist_trigger),
    m_overtake_done_dist(ref.m_overtake_done_dist),
    m_merge_min_dist(ref.m_merge_min_dist),
    m_search_radius_around(ref.m_search_radius_around),
    m_search_radius_to_car_in_front(ref.m_search_radius_to_car_in_front),
    m_ramp_meter_period(ref.m_ramp_meter_period),
    m_ramp_meter(ref.m_ramp_meter),
    probs(ref.probs),
    m_multiplier(ref.m_multiplier)
{
    // clear values if there are any.
    for(Car * delete_this : m_cars){
        delete delete_this;
    }
    m_cars.clear();

    // reserve place for new pointers.
    m_cars.reserve(ref.m_cars.size());

    // copy values into new pointers
    for(Car * car : ref.m_cars){
        Car * new_car_pointer = new Car(*car);
        //*new_car_pointer = *car;
        m_cars.push_back(new_car_pointer);
    }

    // values we copied are good, except the car pointers inside the car class.
    std::map<int,Car*> overtake_this_car;
    std::map<Car*,int> labeling;
    for(int i = 0; i < m_cars.size(); i++){
        overtake_this_car[i] = ref.m_cars[i]->overtake_this_car;
        labeling[ref.m_cars[i]] = i;
        m_cars[i]->overtake_this_car = nullptr; // clear copied pointers
        //m_cars[i]->want_to_overtake_me.clear(); // clear copied pointers
    }
    std::map<int,int> from_to;
    for(int i = 0; i < m_cars.size(); i++){
        if(overtake_this_car[i] != nullptr){
            from_to[i] = labeling[overtake_this_car[i]];
        }
    }

    for(auto it : from_to){
        m_cars[it.first]->overtake_this_car = m_cars[it.second];
        //m_cars[it.second]->want_to_overtake_me.push_back(m_cars[it.first]);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// Copy-assignment constructor, deep copies all content and swaps.

Traffic& Traffic::operator=(const Traffic & rhs) {
    Traffic tmp(rhs);

    std::swap(debug,tmp.debug);
    std::swap(m_font,tmp.m_font);
    std::swap(m_cars,tmp.m_cars);
    std::swap(m_multiplier,tmp.m_multiplier);
    std::swap(probs,tmp.probs);

    return *this;
}

////////////////////////////////////////////////////////////////////////////////
/// Destructor, deletes all cars.

Traffic::~Traffic() {
    for(Car * & car : m_cars){
        delete car;
    }
    Traffic::m_cars.clear();
}

////////////////////////////////////////////////////////////////////////////////
/// Returns size of car vector

unsigned long Traffic::n_of_cars(){
    return m_cars.size();
}

////////////////////////////////////////////////////////////////////////////////
/// Random generator, returns reference to random generator in order to,
/// not make unneccesary copies.

std::mt19937& Traffic::my_engine() {
    static std::mt19937 e(std::random_device{}());
    return e;
}

////////////////////////////////////////////////////////////////////////////////
/// Logic for spawning cars by looking at how much time has elapsed.
/// @param spawn_counter : culmulative time elapsed
/// @param elapsed : time elapsed for one time step.
/// @param threshold : threshold is set by randomly selecting a poission
/// distributed number.
///
/// Cars that are spawned are poission distributed in time, the speed of the
/// cars are normally distributed according to their aggresiveness.

void Traffic::spawn_cars(std::vector<double*> & spawn_counter, float elapsed) {
    int i = 0;
    std::vector<RoadSegment*> segments = Road::shared().spawn_positions();
    std::vector<Car *> cars;
    for(int j = 0; j < 4; j++){
        cars.push_back(nullptr);
    }

    for(double * counter : spawn_counter){
        if(*counter < 0){
            std::gamma_distribution<double> dis(m_spawn_freq,probs[i]);
            std::normal_distribution<float> aggro(m_aggro,m_aggro_sigma);

            *counter = dis(my_engine());
            float aggressiveness = aggro(my_engine());
            float speed = m_speed*aggressiveness;
            float target = speed;

            if(i < 3){
                Car * new_car = new Car(segments[0],i,speed,target,aggressiveness,m_min_dist_to_car_in_front,
                                  m_min_overtake_dist_trigger,m_max_overtake_dist_trigger,m_overtake_done_dist,
                                  m_merge_min_dist,m_search_radius_around,m_search_radius_to_car_in_front);
                cars[i] = new_car;
            }
            else{
                Car * new_car = new Car(segments[1],0,speed,target,aggressiveness,m_min_dist_to_car_in_front,
                                  m_min_overtake_dist_trigger,m_max_overtake_dist_trigger,m_overtake_done_dist,
                                  m_merge_min_dist,m_search_radius_around,m_search_radius_to_car_in_front);
                cars[i] = new_car;
            }
        }
        i++;
        *counter -= elapsed;
    }

    for(Car * car : cars) {
        if(car != nullptr){
            Car * closest_car_ahead = car->find_closest_car_ahead();

            if(closest_car_ahead == nullptr && closest_car_ahead != car){
                m_cars.push_back(car);
            }
            else{
                float dist = Util::distance_to_car(car,closest_car_ahead);
                if(dist < 10){
                    delete car;
                }
                else if (dist < 150){
                    car->speed() = closest_car_ahead->speed();
                    m_cars.push_back(car);
                }
                else{
                    m_cars.push_back(car);
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
/// Despawn @param car

void Traffic::despawn_car(Car *& car) {
    unsigned long size = m_cars.size();
    for(int i = 0; i < size; i++){
        if(car == m_cars[i]){
            //std::cout << "found " << car << "," << m_cars[i] << std::endl;
            delete m_cars[i];
            m_cars[i] = nullptr;
            //std::cout << car << std::endl;
            m_cars.erase(m_cars.begin()+i);
            car = nullptr;
            //std::cout << "deleted\n";
            break;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
/// Despawn cars that are in the despawn segment.

void Traffic::despawn_cars() {
    //std::cout << "e\n";
    std::map<Car *, bool> to_delete;
    for(Car * car : m_cars){
        for(RoadSegment * seg : Road::shared().despawn_positions()){
            if(car->get_segment() == seg){

                to_delete[car] = true;
                break;
            }
        }
    }

    for(Car * car : m_cars){
        for(auto it : to_delete){
            if(it.first == car->overtake_this_car){
                car->overtake_this_car = nullptr;
            }
        }
    }

    for(Car * & car : m_cars){
        if(to_delete[car]){
            delete car;
            car = nullptr;
        }
    }

    //std::cout << "f\n";
    std::vector<Car*>::iterator new_end = std::remove(m_cars.begin(),m_cars.end(),static_cast<Car*>(nullptr));
    m_cars.erase(new_end,m_cars.end());
    //std::cout << "g\n";
}

////////////////////////////////////////////////////////////////////////////////
/// Despawn all cars.

void Traffic::despawn_all_cars() {
    for(Car * car : m_cars){
        car->overtake_this_car = nullptr;
    }

    for(Car * & car : m_cars){
        delete car;
        car = nullptr;
    }

    m_cars.clear();
}

////////////////////////////////////////////////////////////////////////////////
/// Force places a new car with user specified inputs.
///
/// \param seg : segment of car
/// \param node : node of car
/// \param vel : (current)velocity of car
/// \param target : target velocity of car
/// \param aggro : agressiveness of car

void Traffic::force_place_car(RoadSegment * seg, RoadNode * node, float vel, float target, float aggro) {
    Car * car = new Car(seg,node,vel,target,aggro,m_min_dist_to_car_in_front,
                        m_min_overtake_dist_trigger,m_max_overtake_dist_trigger,m_overtake_done_dist,
                        m_merge_min_dist,m_search_radius_around,m_search_radius_to_car_in_front);
    m_cars.push_back(car);
}

////////////////////////////////////////////////////////////////////////////////
/// Updates traffic according by stepping @param elapsed_time seconds in time.

void Traffic::update(float elapsed_time) {
    if(m_ramp_meter){
        float temp = Road::shared().ramp_meter_position->ramp_counter;
        temp += elapsed_time;
        if(temp >= m_ramp_meter_period){
            temp -= m_ramp_meter_period;
            Road::shared().ramp_meter_position->car_passed = false;
        }
        Road::shared().ramp_meter_position->ramp_counter = temp;
    }

    for(Car * & car : m_cars){
        car->avoid_collision(elapsed_time);
    }

    for(Car * & car : m_cars){
        car->update_pos(elapsed_time);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// Returns vector of all cars.

std::vector<Car *> Traffic::get_car_copies() const {
    return m_cars;
}

////////////////////////////////////////////////////////////////////////////////
/// Returns average flow of all cars. Average value of
/// quotient of current speed divided by target speed for all cars.

float Traffic::get_avg_flow() {
    float flow = 0;
    float i = 0;
    for(Car * car : m_cars){
        i++;
        flow += car->speed()/car->target_speed();
    }
    if(m_cars.empty()){
        return 0;
    }
    else{
        return flow/i;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// Returns average speeds of all cars in km/h. First entry in vector
/// is average speed of all cars, second entry is average speed of cars in left
/// lane, third entry is average speed of cars in right lane.

std::vector<float> Traffic::get_avg_speeds() {
    std::vector<float> speedy;
    speedy.reserve(3);

    float flow = 0;
    float flow_left = 0;
    float flow_right = 0;
    float i = 0;
    float j = 0;
    float k = 0;
    for(Car * car : m_cars){
        i++;
        flow += car->speed()*3.6f;

        if(car->current_segment->get_total_amount_of_lanes() == 2){
            if(car->current_segment->get_lane_number(car->current_node) == 1){
                flow_left += car->speed()*3.6f;
                j++;
            }
            else{
                flow_right += car->speed()*3.6f;
                k++;
            }
        }
    }
    if(m_cars.empty()){
        return speedy;
    }
    else{
        flow =  flow/i;
        flow_left = flow_left/j;
        flow_right = flow_right/k;
        speedy.push_back(flow);
        speedy.push_back(flow_left);
        speedy.push_back(flow_right);
        return speedy;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// Draws cars (and nodes if debug = true) to @param target, which could
/// be a window. Blue cars are cars that want to overtake someone,
/// green cars are driving as fast as they want (target speed),
/// red cars are driving slower than they want.

void Traffic::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    // print debug info about node placements and stuff

    sf::CircleShape circle;
    circle.setRadius(4.0f);
    circle.setOutlineColor(sf::Color::Cyan);
    circle.setOutlineThickness(1.0f);
    circle.setFillColor(sf::Color::Transparent);

    sf::Text segment_n;
    segment_n.setFont(m_font);
    segment_n.setFillColor(sf::Color::Black);
    segment_n.setCharacterSize(14);

    sf::VertexArray line(sf::Lines,2);
    line[0].color = sf::Color::Blue;
    line[1].color = sf::Color::Blue;

    if(debug){
        int i = 0;

        for(RoadSegment * segment : Road::shared().segments()){
            for(RoadNode * node : segment->get_nodes()){
                circle.setPosition(sf::Vector2f(node->get_x()*2-4,node->get_y()*2-4));
                line[0].position = sf::Vector2f(node->get_x()*2,node->get_y()*2);
                for(RoadNode * connected_node : node->get_nodes_from_me()){
                    line[1].position = sf::Vector2f(connected_node->get_x()*2,connected_node->get_y()*2);
                    target.draw(line,states);
                }
                target.draw(circle,states);

            }
            segment_n.setString(std::to_string(i));
            segment_n.setPosition(sf::Vector2f(segment->get_x()*2+4,segment->get_y()*2+4));
            target.draw(segment_n,states);

            i++;
        }
    }
    if(m_ramp_meter){
        RoadSegment * meter = Road::shared().ramp_meter_position;
        circle.setPosition(sf::Vector2f(meter->get_x()*2+4-25,meter->get_y()*2-4));
        circle.setOutlineColor(sf::Color::Black);
        if(meter->ramp_counter > m_ramp_meter_period*0.5f){
            circle.setFillColor(sf::Color::Green);

        }
        else{
            circle.setFillColor(sf::Color::Red);
        }
        target.draw(circle,states);
        circle.setOutlineColor(sf::Color::Cyan);
        circle.setFillColor(sf::Color::Transparent);
    }

    // one rectangle is all we need :)
    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(9.4,3.4));
    //rectangle.setFillColor(sf::Color::Green);
    rectangle.setOutlineColor(sf::Color::Black);
    rectangle.setOutlineThickness(2.0f);

    //std::cout << "start drawing\n";
    for(Car * car : m_cars){
        if(car != nullptr){
            //std::cout << "a\n";
            rectangle.setPosition(car->x_pos()*2,car->y_pos()*2);
            rectangle.setRotation(car->theta()*(float)360.0f/(-2.0f*(float)M_PI));
            unsigned int colval = (unsigned int)std::min(255.0f*(car->speed()/car->target_speed()),255.0f);
            sf::Uint8 colorspeed = static_cast<sf::Uint8> (colval);
            //std::cout << "b\n";
            if(car->overtake_this_car != nullptr){
                rectangle.setFillColor(sf::Color(255-colorspeed,0,colorspeed,255));
            }
            else{
                rectangle.setFillColor(sf::Color(255-colorspeed,colorspeed,0,255));
            }

            target.draw(rectangle,states);

            // this caused crash earlier
            if(car->heading_to_node!=nullptr && debug){
                // print debug info about node placements and stuff
                circle.setOutlineColor(sf::Color::Red);
                circle.setOutlineThickness(2.0f);
                circle.setFillColor(sf::Color::Transparent);
                circle.setPosition(sf::Vector2f(car->current_node->get_x()*2-4,car->current_node->get_y()*2-4));
                target.draw(circle,states);
                circle.setOutlineColor(sf::Color::Green);
                circle.setPosition(sf::Vector2f(car->heading_to_node->get_x()*2-4,car->heading_to_node->get_y()*2-4));
                target.draw(circle,states);
            }
        }
    }
    //std::cout << "stop drawing\n";
}

////////////////////////////////////////////////////////////////////////////////
/// Modifies @param text by inserting information about Traffic,
/// average speeds and frame rate among other things.

void Traffic::get_info(sf::Text & text,sf::Time &elapsed) {
    //TODO: SOME BUG HERE.

    float fps = 1.0f/elapsed.asSeconds();
    unsigned long amount_of_cars = n_of_cars();
    float flow = get_avg_flow();
    std::vector<float> spe = get_avg_speeds();
    std::string speedy = std::to_string(fps).substr(0,2) +
                         " fps, ncars: " + std::to_string(amount_of_cars) + "\n"
                         + "avg_flow: " + std::to_string(flow).substr(0,4) +"\n"
                         + "avg_speed: " + std::to_string(spe[0]).substr(0,5) +"km/h\n"
                         + "left_speed: " + std::to_string(spe[1]).substr(0,5) +"km/h\n"
                         + "right_speed: " + std::to_string(spe[2]).substr(0,5) +"km/h\n"
                         + "sim_multiplier: " + std::to_string(m_multiplier).substr(0,3) + "x";
    text.setString(speedy);
    text.setPosition(0,0);
    text.setFillColor(sf::Color::Black);
    text.setFont(m_font);
}

