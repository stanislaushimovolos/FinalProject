#pragma once

#include <SFML/System.hpp>
#include <SFML/Network/Packet.hpp>
#include "../../configuration/config.h"

namespace ser
{

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

    virtual void to_packet(sf::Packet &pack) const = 0;

    virtual void set_direction(uint32_t new_direction);

    void set_velocity(sf::Vector2f &new_velocity);

    virtual void update() = 0;

    void move(sf::Vector2f &shift);

    uint32_t get_direction() const;

    uint32_t get_rotation() const;

    uint32_t get_type() const;

    sf::Vector2f get_position() const;

    virtual ~GameObject() = default;


 protected:
    sf::Vector2f _position;
    sf::Vector2f _velocity;
    float _speed;

    uint32_t _direction;
    uint32_t _rotation;

    uint32_t _type;
};

}