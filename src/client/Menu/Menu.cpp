#include "Menu.h"


Menu::Menu(float width, float height) :
    _width(width),
    _height(height)
{
    if (!font.loadFromFile("../client/Menu/arial.ttf"))
    {
        // handle error
    }
}


void Menu::set_lines(const std::vector<std::string> &lines)
{
    number_of_lines = lines.size();
    menu.resize(number_of_lines);
    for (int i = 0; i < lines.size(); i++)
    {
        menu[i].setFont(font);
        menu[i].setColor(sf::Color::White);
        menu[i].setString(lines[i]);
        menu[i].setPosition(sf::Vector2f(_width / 2, _height / (number_of_lines + 1) * (i + 1)));
    }

    menu[0].setColor(sf::Color::Red);
    selectedItemIndex = 0;
}


int Menu::update(sf::RenderWindow &window)
{
    sf::Event event;
    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::KeyReleased:
                {
                    switch (event.key.code)
                    {
                        case sf::Keyboard::Up:
                        {
                            MoveUp();
                            break;
                        }
                        case sf::Keyboard::Down:
                        {
                            MoveDown();
                            break;
                        }
                        case sf::Keyboard::Return:
                        {
                            return GetPressedItem();
                        }
                        default: break;
                    }
                    break;
                }
                case sf::Event::Closed:
                {
                    window.close();
                    return -1;
                }
                default:break;

            }
        }

        draw(window);
    }
}


void Menu::draw(sf::RenderWindow &window)
{
    window.clear();
    for (auto &txt: menu)
    {
        window.draw(txt);
    }
    window.display();
}


void Menu::MoveUp()
{
    if (selectedItemIndex >= 1)
    {
        menu[selectedItemIndex].setColor(sf::Color::White);
        selectedItemIndex--;
        menu[selectedItemIndex].setColor(sf::Color::Red);
    }
}


void Menu::MoveDown()
{
    if (selectedItemIndex + 1 < number_of_lines)
    {
        menu[selectedItemIndex].setColor(sf::Color::White);
        selectedItemIndex++;
        menu[selectedItemIndex].setColor(sf::Color::Red);
    }
}

