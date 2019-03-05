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
    bool pressing;

    bool is_mouse_in_rect(sf::RenderWindow & App);
public:
    Button(sf::Font & font_copy, unsigned int font_size, int x_pos, int y_pos, const std::string & name, sf::Color button_col, sf::Color text_col, sf::Color pressed);

    void center_text();
    void set_origin(int x, int y);
    void set_dim(int x, int y);
    bool clicked(sf::RenderWindow & App);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif //HIGHWAY_BUTTON_H
