//
// Created by Carl Schiller on 2019-03-06.
//

#include "screen3.h"
#include "button.h"
#include <unistd.h>
#include <iostream>
#include "traffic.h"
#include "simulation2.h"

screen_3::screen_3() {
    run_bool = false;
};

int screen_3::Run(sf::RenderWindow &App, std::vector<float> * args,std::vector<bool> * bargs) {
    sf::Color normal = sf::Color(253,246,227);
    sf::Color hover = sf::Color(253,235,227);

    sf::Sprite sprite;
    sf::Texture texture;
    sf::Font font;

    if(!texture.loadFromFile("../iu.png")){
        return -1;
    }
    if(!font.loadFromFile("/usr/share/fonts/TTF/OpenSans-Regular.ttf")){
        return -1;
    }

    sf::Event event;

    bool just_arrived = true;

    sprite.setTexture(texture);
    sprite.setColor(sf::Color::White);
    sprite.setScale(App.getSize().x/sprite.getLocalBounds().width,App.getSize().y/sprite.getLocalBounds().height);

    Button button1 = Button(font,28*2,500,500,"Go back",normal,sf::Color::Black,hover);
    button1.set_origin(0,0);
    button1.set_dim(App.getSize().x,100);
    button1.center_text();

    std::string stri = "Simulate for (seconds): ";

    std::vector<Input *> inputs;

    Input * input = new Input(font,28,500,500,stri,normal,sf::Color::Black,hover,sf::Color(240,255,255,255),std::to_string(1000));
    input->set_origin(0,button1.get_bounds().height+button1.getPosition().y);
    input->set_dim(App.getSize().x,50);
    input->center_text();

    inputs.push_back(input);

    Button button2 = Button(font,28*2,500,500,"Run simulation",normal,sf::Color::Black,hover);
    button2.set_origin(0,input->get_bounds().height+input->get_pos().y);
    button2.set_dim(App.getSize().x,100);
    button2.center_text();

    Input * current_input = nullptr;

    int micro = 1000000;
    usleep((useconds_t)micro/8);

    int * percent = new int;
    *percent = 0;

    auto traffic = new Traffic(*bargs,*args);
    frame_rate = (int)args[0][16];
    Sim sim = Sim(traffic,frame_rate,&sim_time,&run_bool,percent);

    sim_time = 1000;
    sf::Thread thread(&Sim::update,&sim);

    bool stop_bool = false;

    while(true) {

        while (App.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                return -1;
            }

            if (event.type == sf::Event::MouseButtonPressed && just_arrived) {

            } else if (!just_arrived && current_input == nullptr && !stop_bool) {
                if (button1.clicked(App)) {
                    delete traffic;
                    delete percent;
                    return 0;
                }
                else if (button2.clicked(App)) {
                    // launch thread here...
                    sim_time = (int)input->get_val();
                    stop_bool = true;
                    thread.launch();
                    button2.set_text("Wait...");

                }
                for (Input *inp : inputs) {
                    current_input = inp->clicked(App);
                    if (current_input != nullptr) {
                        break;
                    }
                }
            } else {
                just_arrived = false;
            }

            if (event.type == sf::Event::TextEntered && current_input != nullptr) {
                sf::String str = event.text.unicode;
                current_input = current_input->inputing(App, str);
            }
        }

        App.clear();

        App.draw(sprite);

        App.draw(button1);
        for (Input *inp : inputs) {
            App.draw(*inp);
        }

        if(stop_bool){
            button2.set_text("Wait ..." + std::to_string(*percent) + "%");
        }
        App.draw(button2);


        App.display();

        if(run_bool){
            thread.terminate();
            run_bool = false;
            stop_bool = false;
            button2.set_text("Run simulation");
        }
    }
}

