#include "GameObject.h"

namespace ser
{

GameObject::GameObject(sf::Vector2f pos,
                       sf::Vector2f velocity,
                       uint32_t start_rotation,
                       uint32_t start_direction,
                       float speed,
                       uint32_t type) :
    _position(pos),
    _velocity(velocity),
    _speed(speed),
    _rotation(start_rotation),
    _direction(start_direction),
    _type(type)
{}


void GameObject::move(sf::Vector2f &shift)
{
    _position += shift;
}


void GameObject::set_velocity(sf::Vector2f &new_velocity)
{
    _velocity = new_velocity;
}


void GameObject::set_direction(uint32_t new_direction)
{
    _direction = new_direction;
    switch (new_direction)
    {
        default:break;
    }

    if (new_direction != conf::game::Rest)
        _rotation = new_direction;
}


uint32_t GameObject::get_direction() const
{
    return _direction;
}


uint32_t GameObject::get_rotation() const
{
    return _rotation;
}


uint32_t GameObject::get_type() const
{
    return _type;
}


sf::Vector2f GameObject::get_position() const
{
    return _position;
}

}