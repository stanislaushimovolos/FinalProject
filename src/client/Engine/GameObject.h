#pragma once

#include <SFML/Graphics.hpp>
#include "../../configuration/config.h"

namespace cli
{
class GameObject
{
 public:

    GameObject(sf::Vector2f &&pos, uint32_t type);

    GameObject();

    virtual void set_state_form_packet(sf::Packet &pack) = 0;

    virtual void draw(sf::RenderWindow &window) = 0;

    void set_position(sf::Vector2f &pos);

    void set_position(sf::Vector2f &&pos);

    virtual ~GameObject() = default;

    sf::Vector2f get_position() const;

 protected:
    uint32_t _type;
    sf::Vector2f _position;

};

}
