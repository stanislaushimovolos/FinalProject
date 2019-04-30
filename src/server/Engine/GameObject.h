#pragma once

#include <SFML/System.hpp>
#include <SFML/Network/Packet.hpp>
#include "Properties.h"
#include "../../configuration/config.h"

namespace ser
{

class Property;

// abstract class for all game entities
class GameObject
{
 public:

    explicit GameObject(sf::Vector2f pos,
                        sf::Vector2f velocity,
                        uint32_t start_rotation,
                        uint32_t start_direction,
                        float speed,
                        uint32_t type);

    GameObject() = delete;

    virtual void compress_to_packet(sf::Packet &pack) const = 0;

    virtual void set_direction(uint32_t new_direction);

    virtual void update(int delta_t) = 0;

    void set_speed_from_direction(uint32_t new_direction);

    void set_velocity(sf::Vector2f &new_velocity);

    void add_property(Property *prop);

    void move(sf::Vector2f &shift);

    uint32_t get_direction() const;

    uint32_t get_rotation() const;

    uint32_t get_type() const;

    sf::Vector2f get_position() const;

    virtual ~GameObject() = default;


 protected:
    std::vector<Property *> _properties;

    sf::Vector2f _position;
    sf::Vector2f _velocity;
    float _speed;

    uint32_t _direction;
    uint32_t _rotation;

    uint32_t _type;
};

}