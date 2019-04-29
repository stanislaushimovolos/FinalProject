#include "KeyboardInterface.h"

namespace cli
{

KeyboardInterface::KeyboardInterface() :
    _direction(conf::game::Rest)
{}


std::pair<uint32_t, uint32_t> KeyboardInterface::get_direction() const
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

}
