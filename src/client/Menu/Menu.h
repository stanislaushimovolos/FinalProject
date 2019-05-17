#pragma once
#include <iostream>
#include "SFML/Graphics.hpp"

class Menu
{
 public:
    Menu(float width, float height);

    int update(sf::RenderWindow &window);

    void draw(sf::RenderWindow &window);

    void set_lines(const std::vector<std::string> &lines);

    void MoveUp();

    void MoveDown();

    int GetPressedItem()
    {
        return selectedItemIndex;
    }


 private:

    std::vector<sf::Text> menu;
    int selectedItemIndex;
    sf::Font font;

    size_t number_of_lines;
    float _width, _height;

};

