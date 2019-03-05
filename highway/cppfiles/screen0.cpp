//
// Created by Carl Schiller on 2019-03-04.
//

#include "../headers/screen0.h"
#include <iostream>
#include "button.h"

screen_0::screen_0() = default;

int screen_0::Run(sf::RenderWindow &App) {
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

    sprite.setTexture(texture);
    sprite.setColor(sf::Color::White);
    sprite.setScale(App.getSize().x/sprite.getLocalBounds().width,App.getSize().y/sprite.getLocalBounds().height);

    Button button1 = Button(font,28*2,500,500,"Visualize simulation",normal,sf::Color::Black,hover);
    button1.set_origin(0,0);
    button1.set_dim(App.getSize().x,100);
    button1.center_text();

    Button button2 = Button(font,28*2,500,500,"Settings",normal,sf::Color::Black,hover);
    button2.set_origin(0,100);
    button2.set_dim(App.getSize().x,100);
    button2.center_text();

    std::vector<Button *> buttons;

    buttons.push_back(&button1);
    buttons.push_back(&button2);

    while(true){
        while(App.pollEvent(event)){
            if(event.type == sf::Event::Closed){
                return -1;
            }
        }

        for (Button * but : buttons) {
            if(but->clicked(App)){
                return 1;
            }
        }


        App.clear();

        App.draw(sprite);

        for (Button * but : buttons) {
            App.draw(*but);
        }

        App.display();
    }
}