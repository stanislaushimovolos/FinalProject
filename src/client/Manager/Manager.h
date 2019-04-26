#pragma once

#include <SFML/Graphics.hpp>
#include "../../configuration/config.h"
#include "../Engine/GameObject.h"
#include "../GameEntities/Player.h"
#include "../GameEntities/Bullet.h"

namespace cli
{

class KeyboardInterface
{
 private:
    uint32_t _direction;

 public:
    std::pair<uint32_t, uint32_t> get_direction() const
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

        uint32_t is_shoot = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            is_shoot = 1;

        return std::make_pair(direction, is_shoot);
    }

    KeyboardInterface() :
        _direction(conf::game::Rest)
    {}
};

class Manager
{
 public:
    Manager(uint32_t x_resolution, uint32_t y_resolution, std::string &&window_name);

    void set_ip_port(std::pair<uint32_t , uint32_t > ip_port);

    void update(sf::Packet &packet);

    void activate_window();

    sf::Packet get_current_state();

    bool is_active();

    void draw();

    /// LOOOOOLL!!!!!!!!!!!!
    sf::View _view;

    ~Manager();

 private:

    // Window parameters
    sf::RenderWindow _window;

    sf::Vector2u _resolution;
    std::string _window_name;
    bool _is_window_focused;
    bool _is_window_opened;

    uint32_t port, ip;

    uint32_t _current_num_of_objects;
    KeyboardInterface keyboard;

    std::vector<GameObject *> _objects;

    int process_scene(sf::Packet &packet);
};

}
