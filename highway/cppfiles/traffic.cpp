//
// Created by Carl Schiller on 2018-12-19.
//

#include "../headers/traffic.h"
#include "../headers/car.h"
#include "../headers/road.h"
#include "../headers/util.h"

////////////////////////////////////////////////////////////////////////////////
/// Constructor.

Traffic::Traffic() {
    debug = false;
    if(!m_font.loadFromFile("/Library/Fonts/Arial.ttf")){
        //crash
    }
}

Traffic::Traffic(bool debug) : debug(debug){
    if(!m_font.loadFromFile("/Library/Fonts/Arial.ttf")){
        //crash
    }
}

Traffic::Traffic(const Traffic &ref) :
    m_multiplier(ref.m_multiplier), debug(ref.debug)
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
        auto new_car_pointer = new Car;
        *new_car_pointer = *car;
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

Traffic& Traffic::operator=(const Traffic & rhs) {
    Traffic tmp(rhs);

    std::swap(m_cars,tmp.m_cars);
    std::swap(m_multiplier,tmp.m_multiplier);
    std::swap(debug,tmp.debug);

    return *this;
}

Traffic::~Traffic() {
    for(int i = 0; i<m_cars.size(); i++){
        delete Traffic::m_cars[i];
    }
    Traffic::m_cars.clear();
}

unsigned long Traffic::n_of_cars(){
    return m_cars.size();
}

std::mt19937& Traffic::my_engine() {
    static std::mt19937 e(std::random_device{}());
    return e;
}

void Traffic::spawn_cars(double & spawn_counter, float elapsed, double & threshold) {
    spawn_counter += elapsed;
    if(spawn_counter > threshold){
        std::exponential_distribution<double> dis(5);
        std::normal_distribution<float> aggro(1.0f,0.2f);
        float sp = 30.0f;
        std::uniform_real_distribution<float> lane(0.0f,1.0f);
        std::uniform_real_distribution<float> spawn(0.0f,1.0f);

        threshold = dis(my_engine());
        float aggressiveness = aggro(my_engine());
        float speed = sp*aggressiveness;
        float target = speed;

        spawn_counter = 0;
        float start_lane = lane(my_engine());
        float spawn_pos = spawn(my_engine());

        std::vector<RoadSegment*> segments = Road::shared().spawn_positions();
        RoadSegment * seg;
        Car * new_car;
        if(spawn_pos < 0.95){
            seg = segments[0];
            if(start_lane < 0.457){
                new_car = new Car(seg,2,speed,target,aggressiveness);
            }
            else if(start_lane < 0.95){
                new_car = new Car(seg,1,speed,target,aggressiveness);
            }
            else{
                new_car = new Car(seg,0,speed,target,aggressiveness);
            }
        }
        else{
            seg = segments[1];
            new_car = new Car(seg,0,speed,target,aggressiveness);
        }

        Car * closest_car_ahead = new_car->find_closest_car_ahead();

        if(closest_car_ahead == nullptr && closest_car_ahead != new_car){
            m_cars.push_back(new_car);
        }
        else{
            float dist = Util::distance_to_car(new_car,closest_car_ahead);
            if(dist < 10){
                delete new_car;
            }
            else if (dist < 150){
                new_car->speed() = closest_car_ahead->speed();
                m_cars.push_back(new_car);
            }
            else{
                m_cars.push_back(new_car);
            }
        }
    }
}

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

void Traffic::despawn_all_cars() {
    *this = Traffic();
}

void Traffic::force_place_car(RoadSegment * seg, RoadNode * node, float vel, float target, float aggro) {
    Car * car = new Car(seg,node,vel,target,aggro);
    m_cars.push_back(car);
}

void Traffic::update(float elapsed_time) {
    //std::cout<< "updatin1\n";
    for(Car * car : m_cars){
        car->avoid_collision(elapsed_time);
    }
    //std::cout<< "updatin2\n";
    for(Car * car : m_cars){
        car->update_pos(elapsed_time);
    }
    //std::cout<< "updatin3\n";
}

std::vector<Car *> Traffic::get_car_copies() const {
    return m_cars;
}

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

    // one rectangle is all we need :)
    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(9.4,3.4));
    //rectangle.setFillColor(sf::Color::Green);
    rectangle.setOutlineColor(sf::Color::Black);
    rectangle.setOutlineThickness(2.0f);

    //std::cout << "start drawing\n";
    for(Car * car : m_cars){
        //std::cout << "drawing" << car << std::endl;
        if(car != nullptr){
            rectangle.setPosition(car->x_pos()*2,car->y_pos()*2);
            rectangle.setRotation(car->theta()*(float)360.0f/(-2.0f*(float)M_PI));
            unsigned int colval = (unsigned int)std::min(255.0f*(car->speed()/car->target_speed()),255.0f);
            sf::Uint8 colorspeed = static_cast<sf::Uint8> (colval);

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
}

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

