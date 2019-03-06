//
// Created by Carl Schiller on 2019-03-05.
//

#ifndef HIGHWAY_BUTTON_H
#define HIGHWAY_BUTTON_H

#include "SFML/Graphics.hpp"
#include <string>

class Button : public sf::Drawable, public sf::Transformable{
private:
    sf::Font font;
    sf::RectangleShape rect;
    sf::Text text;
    sf::Color normal;
    sf::Color pressed;

    bool is_mouse_in_rect(sf::RenderWindow & App);
public:
    Button(sf::Font & font_copy, unsigned int font_size, int x_pos, int y_pos, const std::string & name, sf::Color button_col, sf::Color text_col, sf::Color pressed);

    void center_text();
    void set_origin(int x, int y);
    void set_dim(int x, int y);
    bool clicked(sf::RenderWindow & App);
    sf::FloatRect get_bounds();

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};


class Input : public sf::Drawable, public sf::Transformable{
private:
    sf::Font font;
    sf::RectangleShape rect;
    sf::Text text;
    std::string string;
    std::string input;
    sf::Color normal;
    sf::Color pressed;
    sf::Color typing;

    bool bool_typing;

public:

    Input(sf::Font & font_copy, unsigned int font_size, int x_pos, int y_pos,
            const std::string & name, sf::Color button_col, sf::Color text_col, sf::Color pressed,
            sf::Color typing, std::string val);

    bool is_mouse_in_rect(sf::RenderWindow & App);
    void center_text();
    void set_origin(int x, int y);
    void set_dim(int x, int y);
    virtual Input * clicked(sf::RenderWindow & App);
    Input * inputing(sf::RenderWindow & App, std::string & str);
    float get_val();
    sf::FloatRect get_bounds();
    const sf::Vector2f get_pos();
    friend class Button_bool;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

class Button_bool : public Input {
    bool toggled;
public:
    void set_toggled(bool tog);
    using Input::Input;
    virtual Button_bool * clicked(sf::RenderWindow & App);
    bool get_bool();
};

#endif //HIGHWAY_BUTTON_H
