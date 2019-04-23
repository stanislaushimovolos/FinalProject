#pragma once

#include <SFML/Graphics.hpp>
#include "../../configuration/config.h"

namespace cli
{
class GameObject
{
 public:
    GameObject();

    GameObject(sf::Vector2f pos, uint32_t type);

    virtual void draw(sf::RenderWindow &window) = 0;

    void set_position(sf::Vector2f pos);

    sf::Vector2f get_position() const;

 protected:
    uint32_t _type;
    sf::Vector2f _position;

};

}
