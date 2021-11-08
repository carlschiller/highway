//
// Created by Carl Schiller on 2019-03-05.
//

#include "screen2.h"
#include <iostream>
#include "button.h"
#include <unistd.h>

screen_2::screen_2() = default;

int screen_2::Run(sf::RenderWindow &App, std::vector<float> * args,std::vector<bool> * bargs) {
    sf::Color normal = sf::Color(253,246,227);
    sf::Color hover = sf::Color(253,235,227);

    sf::Sprite sprite;
    sf::Texture texture;
    sf::Font font;

    if(!texture.loadFromFile("../iu.png")){
        return -1;
    }
    if(!font.loadFromFile("/usr/share/fonts/truetype/open-sans/OpenSans-Regular.ttf")){
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

    std::map<int,std::string> names;

    names[0] = "Aggresiveness: ";
    names[1] = "Aggro sigma: ";
    names[2] = "Global beta: ";
    names[3] = "Speed: ";
    names[4] = "Lane 0 alpha: ";
    names[5] = "Lane 1 alpha: ";
    names[6] = "Lane 2 alpha: ";
    names[7] = "Ramp 0 alpha: ";
    names[8] = "Min car distance: ";
    names[9] = "Min overtake dist: ";
    names[10] = "Max overtake dist: ";
    names[11] = "Overtake dist shutoff: ";
    names[12] = "Min merge dist: ";
    names[13] = "Search radius around: ";
    names[14] = "Search radius front: ";
    names[15] = "Sim multiplier: ";
    names[16] = "Framerate: ";
    names[17] = "Ramp meter period: ";

    std::vector<Input*> inputs;

    Input * input = new Input(font,28,500,500,names[0],normal,sf::Color::Black,hover,sf::Color(240,255,255,255),std::to_string(args[0][0]));
    input->set_origin(0,button1.get_bounds().height);
    input->set_dim(App.getSize().x,40);
    input->center_text();

    inputs.push_back(input);

    for(int i = 1; i < args->size(); i++){
        input = new Input(font,28,500,500,names[i],normal,sf::Color::Black,hover,sf::Color(240,255,255,255),std::to_string(args[0][i]));
        input->set_origin(0,inputs[i-1]->get_bounds().height+inputs[i-1]->get_pos().y);
        input->set_dim(App.getSize().x,40);
        input->center_text();
        inputs.push_back(input);
    }

    Button_bool bool_button = Button_bool(font,28,500,500,"Debug: ",normal,sf::Color::Black,hover,sf::Color::White,"false");
    bool_button.set_origin(0,input->get_bounds().height + input->get_pos().y);
    bool_button.set_dim(App.getSize().x,40);
    bool_button.set_toggled(bargs[0][0]);
    bool_button.center_text();

    Button_bool bool_button1 = Button_bool(font,28,500,500,"Ramp meter: ",normal,sf::Color::Black,hover,sf::Color::White,"false");
    bool_button1.set_origin(0,bool_button.get_bounds().height + bool_button.get_pos().y);
    bool_button1.set_dim(App.getSize().x,40);
    bool_button1.set_toggled(bargs[0][1]);
    bool_button1.center_text();

    Input * current_input = nullptr;

    int micro = 1000000;
    usleep((useconds_t)micro/8);

    while(true){

        while(App.pollEvent(event)){
            if(event.type == sf::Event::Closed){
                return -1;
            }

            if(event.type == sf::Event::MouseButtonPressed && just_arrived){

            }
            else if(!just_arrived && current_input == nullptr){
                if(button1.clicked(App)){
                    int i = 0;
                    for(Input * inp : inputs){
                        args[0][i] = inp->get_val();
                        i++;
                    }
                    bargs[0][0] = bool_button.get_bool();
                    bargs[0][1] = bool_button1.get_bool();

                    return 0;
                }
                for(Input * inp : inputs){
                    current_input = inp->clicked(App);
                    if(current_input != nullptr){
                        break;
                    }
                }
                bool_button.clicked(App);
                bool_button1.clicked(App);
            }
            else{
                just_arrived = false;
            }

            if(event.type == sf::Event::TextEntered && current_input != nullptr){
                sf::String str = event.text.unicode;
                current_input = current_input->inputing(App,str);
            }
        }

        App.clear();

        App.draw(sprite);

        App.draw(button1);
        for(Input * inp : inputs){
            App.draw(*inp);
        }
        App.draw(bool_button);
        App.draw(bool_button1);

        App.display();
    }
}