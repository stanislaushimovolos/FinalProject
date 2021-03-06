#pragma once

#include <SFML/System.hpp>
#include <SFML/Network/Packet.hpp>

#include "Properties.h"
#include "RectangleCollider.h"
#include "../../map/Level.h"
#include "../../configuration/gameplay.h"

namespace ser
{

class GraphProperty;

// Abstract class for all game entities
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

    virtual void interact(GameObject *obj, int delta_t) = 0;

    virtual void set_direction(uint32_t new_direction);

    virtual void update(int delta_t);

    void compress_properties_to_packet(sf::Packet &pack) const;

    void set_speed_from_direction(uint32_t new_direction);

    void add_property(uint32_t key, GraphProperty *prop);

    void set_velocity(sf::Vector2f &new_velocity);

    void move(sf::Vector2f &&shift);

    void set_status(bool status);

    uint32_t get_direction() const;

    uint32_t get_rotation() const;

    uint32_t get_type() const;

    bool is_active() const;

    const RectangleCollider &get_collider() const;

    sf::Vector2f get_position() const;

    sf::Vector2f get_velocity() const;

    virtual ~GameObject() = default;


 protected:

    std::map<uint32_t, GraphProperty *> _properties;
    RectangleCollider _collider;

    sf::Vector2f _position;
    sf::Vector2f _velocity;
    float _speed;

    uint32_t _direction;
    uint32_t _rotation;

    uint32_t _type;
    bool _is_active;
};

float fast_square_root(float n);

sf::Vector2f operator*(const sf::Vector2f &first, float multiplier);

sf::Vector2f compute_unit_vector(const sf::Vector2f &first, const sf::Vector2f &second);

}