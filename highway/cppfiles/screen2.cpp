//
// Created by Carl Schiller on 2019-03-05.
//

#include "screen2.h"
#include <iostream>
#include "button.h"

screen_2::screen_2() = default;

int screen_2::Run(sf::RenderWindow &App, std::vector<float> * args) {
    sf::Color normal = sf::Color(253,246,227);
    sf::Color hover = sf::Color(253,235,227);

    sf::Sprite sprite;
    sf::Texture texture;
    sf::Font font;

    if(!texture.loadFromFile("../iu.png")){
        return -1;
    }
    if(!font.loadFromFile("/Library/Fonts/Andale mono.ttf")){
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
    names[2] = "Spawn frequency: ";
    names[3] = "Speed: ";
    names[4] = "Spawn prob lane 0: ";
    names[5] = "Spawn prob lane 1: ";
    names[6] = "Spawn prob lane 2: ";
    names[7] = "Spawn prob ramp 0: ";
    names[8] = "Min car distance: ";
    names[9] = "Min overtake dist: ";
    names[10] = "Max overtake dist: ";
    names[11] = "Overtake dist shutoff: ";
    names[12] = "Min merge dist: ";
    names[13] = "Search radius around: ";
    names[14] = "Search radius front: ";
    names[15] = "Sim multiplier: ";
    names[16] = "Framerate: ";

    std::vector<Input*> inputs;

    Input * input = new Input(font,28,500,500,names[0],normal,sf::Color::Black,hover,sf::Color(240,255,255,255),std::to_string(args[0][0]));
    input->set_origin(0,button1.get_bounds().height);
    input->set_dim(App.getSize().x,50);
    input->center_text();

    inputs.push_back(input);

    for(int i = 1; i < args->size(); i++){
        input = new Input(font,28,500,500,names[i],normal,sf::Color::Black,hover,sf::Color(240,255,255,255),std::to_string(args[0][i]));
        input->set_origin(0,inputs[i-1]->get_bounds().height+inputs[i-1]->get_pos().y);
        input->set_dim(App.getSize().x,50);
        input->center_text();
        inputs.push_back(input);
    }

    Input * current_input = nullptr;

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

                    return 0;
                }
                for(Input * inp : inputs){
                    current_input = inp->clicked(App);
                    if(current_input != nullptr){
                        break;
                    }
                }
            }
            else{
                just_arrived = false;
            }

            if(event.type == sf::Event::TextEntered && current_input != nullptr){
                sf::String str = event.text.unicode;
                std::string to_str = str.toAnsiString();
                current_input = current_input->inputing(App,to_str);
            }
        }

        App.clear();

        App.draw(sprite);

        App.draw(button1);
        for(Input * inp : inputs){
            App.draw(*inp);
        }

        App.display();
    }
}