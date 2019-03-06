#include <utility>

//
// Created by Carl Schiller on 2019-03-05.
//

#include <button.h>

Button::Button(sf::Font & font_copy, unsigned int font_size, int x_pos, int y_pos,
        const std::string & name, sf::Color button_col, sf::Color text_col, sf::Color pressed) :
    font(font_copy),
    normal(button_col),
    pressed(pressed)
{
    text.setString(name);
    text.setFont(font);
    text.setCharacterSize(font_size);
    text.setFillColor(text_col);
    text.setOutlineThickness(0);

    sf::FloatRect bounds = text.getLocalBounds();

    rect.setPosition((float)x_pos,(float)y_pos);
    rect.setSize({bounds.width+bounds.left*2,bounds.height+bounds.top*2});
    rect.setFillColor(normal);
    rect.setOutlineThickness(2);
    rect.setOutlineColor(sf::Color::Black);
}

void Button::set_origin(int x, int y) {
    sf::Vector2f box_origin = rect.getOrigin();
    sf::Vector2f text_origin = text.getOrigin();
    sf::Vector2f move_to = {(float)x,(float)y};

    sf::Vector2f diff = text_origin-box_origin;

    rect.setPosition(move_to);
    text.setPosition(diff+move_to);
}

void Button::set_dim(int x, int y) {
    rect.setSize({(float)x,(float)y});
}

void Button::center_text() {
    sf::FloatRect rect_bounds = rect.getLocalBounds();
    sf::FloatRect text_bounds = text.getLocalBounds();

    sf::Vector2f new_origin = {rect_bounds.width/2-(text_bounds.width+text_bounds.left*2)/2,(rect_bounds.height-(text_bounds.height+text_bounds.top*2))/2};
    new_origin = new_origin + rect.getPosition();
    text.setPosition(new_origin);
}

bool Button::clicked(sf::RenderWindow & App) {
    if(is_mouse_in_rect(App)){
        rect.setFillColor(pressed);
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
            rect.setFillColor(normal);
            return true;
        }
        else{
            return false;
        }
    }
    else{
        rect.setFillColor(normal);
        return false;
    }
}

sf::FloatRect Button::get_bounds() {
    return rect.getLocalBounds();
}

bool Button::is_mouse_in_rect(sf::RenderWindow & App) {
    sf::Vector2i rel_to_pos = {sf::Mouse::getPosition(App).x-(int)rect.getPosition().x,sf::Mouse::getPosition(App).y-(int)rect.getPosition().y};
    if(rel_to_pos.x < 0 || rel_to_pos.y < 0){
        return false;
    }
    else if(rect.getLocalBounds().width < rel_to_pos.x || rect.getLocalBounds().height < rel_to_pos.y){
        return false;
    }
    else{
        return true;
    }
}

void Button::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(rect);
    target.draw(text);
}


Input::Input(sf::Font &font_copy, unsigned int font_size, int x_pos, int y_pos, const std::string &name,
             sf::Color button_col, sf::Color text_col, sf::Color pressed, sf::Color typ, std::string val) :
    font(font_copy),
    string(name),
    input(val),
    normal(button_col),
    pressed(pressed),
    typing(typ),
    bool_typing(false)
{
    text.setString(string+input);
    text.setFont(font);
    text.setCharacterSize(font_size);
    text.setFillColor(text_col);
    text.setOutlineThickness(0);

    sf::FloatRect bounds = text.getLocalBounds();

    rect.setPosition((float)x_pos,(float)y_pos);
    rect.setSize({bounds.width+bounds.left*2,bounds.height+bounds.top*2});
    rect.setFillColor(normal);
    rect.setOutlineThickness(2);
    rect.setOutlineColor(sf::Color::Black);
}

void Input::set_origin(int x, int y) {
    sf::Vector2f box_origin = rect.getOrigin();
    sf::Vector2f text_origin = text.getOrigin();
    sf::Vector2f move_to = {(float)x,(float)y};

    sf::Vector2f diff = text_origin-box_origin;

    rect.setPosition(move_to);
    text.setPosition(diff+move_to);
}

Input * Input::clicked(sf::RenderWindow & App) {
    if(!bool_typing){
        if(is_mouse_in_rect(App)){
            rect.setFillColor(pressed);
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                rect.setFillColor(typing);
                bool_typing = true;
                text.setString(string);
                input = "";
                return this;
            }
            else{
                return nullptr;
            }
        }
        else{
            rect.setFillColor(normal);
            return nullptr;
        }
    }
    return this;
}

bool Input::is_mouse_in_rect(sf::RenderWindow & App) {
    sf::Vector2i rel_to_pos = {sf::Mouse::getPosition(App).x-(int)rect.getPosition().x,sf::Mouse::getPosition(App).y-(int)rect.getPosition().y};
    if(rel_to_pos.x < 0 || rel_to_pos.y < 0){
        return false;
    }
    else if(rect.getLocalBounds().width < rel_to_pos.x || rect.getLocalBounds().height < rel_to_pos.y){
        return false;
    }
    else{
        return true;
    }
}

Input * Input::inputing(sf::RenderWindow &App,std::string & str) {
    if(bool_typing){
        if(str == "\n"){
            bool_typing = false;
            rect.setFillColor(normal);
            return nullptr;
        }
        else{
            input += str;
            text.setString(string+input);
            center_text();
            return this;
        }
    }
    return nullptr;
}

float Input::get_val() {
    return std::stof(input);
}

void Input::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(rect);
    target.draw(text);
}

void Input::center_text() {
    sf::FloatRect rect_bounds = rect.getLocalBounds();
    sf::FloatRect text_bounds = text.getLocalBounds();

    sf::Vector2f new_origin = {rect_bounds.width/2-(text_bounds.width+text_bounds.left*2)/2,(rect_bounds.height-(text_bounds.height+text_bounds.top*2))/2};
    new_origin = new_origin + rect.getPosition();
    text.setPosition(new_origin);
}

void Input::set_dim(int x, int y) {
    rect.setSize({(float)x,(float)y});
}

sf::FloatRect Input::get_bounds() {
    return rect.getLocalBounds();
}

const sf::Vector2f Input::get_pos() {
    return rect.getPosition();
}


Button_bool* Button_bool::clicked(sf::RenderWindow &App) {
    if(is_mouse_in_rect(App)){
        rect.setFillColor(pressed);
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
            rect.setFillColor(normal);
            toggled = !toggled;
            text.setString(string+(toggled ? "true" : "false"));
            center_text();
            return this;
        }
        else{
            return nullptr;
        }
    }
    else{
        rect.setFillColor(normal);
        return nullptr;
    }
}

bool Button_bool::get_bool() {
    return toggled;
}

void Button_bool::set_toggled(bool tog) {
    toggled = tog;
    text.setString(string+(toggled ? "true" : "false"));
}