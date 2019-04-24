#pragma once

#include <SFML/System.hpp>
#include <SFML/Network/Packet.hpp>
namespace ser
{

// abstract class for all game entities
class GameObject
{
 public:

    explicit GameObject(sf::Vector2f pos,
                        sf::Vector2f velocity,
                        float speed, uint32_t type);

    GameObject() = delete;

    sf::Vector2f get_position() const;

    virtual void to_packet(sf::Packet &pack) const = 0;

    void move(sf::Vector2f &&shift);

    virtual void update() = 0;

    uint32_t get_type() const;

    virtual ~GameObject() = default;


 protected:
    sf::Vector2f _position;
    sf::Vector2f _velocity;
    float _speed;

    uint32_t _type;
};

}