#pragma once

#include "GameObject.h"
#include <SFML/Graphics.hpp>

namespace ser
{

class GameObject;

class Property
{
 public:

    explicit Property(GameObject *master, uint32_t type);

    virtual void compress_to_packet(sf::Packet &pack) const = 0;

    virtual void update(int delta_t) = 0;

    void set_position(sf::Vector2f &pos);

    uint32_t get_type();

 protected:

    GameObject *_master;
    sf::Vector2f _position;
    uint32_t _type;
};

class SimpleRectangleTexture : public Property
{
 public:

    SimpleRectangleTexture(GameObject *master, sf::Color color, sf::Vector2f &&shape);

    void compress_to_packet(sf::Packet &pack) const override;

    void update(int delta_t) override;

 private:

    uint32_t _color;
    sf::Vector2f _shape;
};

}
