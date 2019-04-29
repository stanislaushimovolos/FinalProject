#include "KeyboardInterface.h"

namespace cli
{

KeyboardInterface::KeyboardInterface() :
    _direction(conf::game::Rest)
{}


// Do better
std::pair<uint32_t, uint32_t> KeyboardInterface::get_direction() const
{
    uint32_t direction = conf::game::Rest;
    uint32_t is_shoot = 0;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
        is_shoot = 1;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            direction = conf::game::UpRight;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            direction = conf::game::DownRight;
        else
            direction = conf::game::Right;

    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            direction = conf::game::UpLeft;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            direction = conf::game::DownLeft;
        else
            direction = conf::game::Left;

    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        direction = conf::game::Up;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        direction = conf::game::Down;
    }
    return std::make_pair(direction, is_shoot);
}

}
