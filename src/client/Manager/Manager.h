#pragma once

#include <SFML/Graphics.hpp>
#include "../../configuration/config.h"
#include "../Engine/GameObject.h"
#include "../Entittes/Player.h"

namespace cli
{

class KeyboardInterface
{
 private:
    uint32_t _direction;

 public:
    uint32_t get_direction() const
    {
        uint32_t direction = conf::game::Rest;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            direction = conf::game::Right;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            direction = conf::game::Left;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            direction = conf::game::Up;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            direction = conf::game::Down;
        }
        return direction;
    }


 public:
    KeyboardInterface() :
        _direction(conf::game::Rest)
    {}
};

class Manager
{
 public:
    Manager(uint32_t x_resolution, uint32_t y_resolution, std::string &&window_name);

    bool is_active();

    void draw();

    void activate_window();

    void update(sf::Packet &packet);

    sf::Packet get_current_state();

 private:

    // Window parameters
    sf::RenderWindow _window;

    sf::Vector2u _resolution;
    std::string _window_name;
    bool _is_window_focused;
    bool _is_window_opened;

    uint32_t _current_num_of_clients;
    KeyboardInterface keyboard;

    std::vector<Player> _objects;

    int process_scene(sf::Packet &packet);
};

}
