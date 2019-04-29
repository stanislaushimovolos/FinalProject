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


void GameObject::set_speed_from_direction(uint32_t new_direction)
{
    _direction = new_direction;
    switch (new_direction)
    {
        case (conf::game::Left):
        {
            _velocity = sf::Vector2f(-_speed, 0);
            break;
        }
        case (conf::game::Right):
        {
            _velocity = sf::Vector2f(_speed, 0);
            break;
        }
        case (conf::game::Up):
        {
            _velocity = sf::Vector2f(0, -_speed);
            break;
        }
        case (conf::game::Down):
        {
            _velocity = sf::Vector2f(0, _speed);
            break;
        }
        case (conf::game::DownRight):
        {
            // sqrt(2)
            _velocity = sf::Vector2f(_speed / 1.4, _speed / 1.4);
            break;
        }
        case (conf::game::UpRight):
        {
            // sqrt(2)
            _velocity = sf::Vector2f(_speed / 1.4, -_speed / 1.4);
            break;
        }
        case (conf::game::DownLeft):
        {
            // sqrt(2)
            _velocity = sf::Vector2f(-_speed / 1.4, _speed / 1.4);
            break;
        }
        case (conf::game::UpLeft):
        {
            // sqrt(2)
            _velocity = sf::Vector2f(-_speed / 1.4, -_speed / 1.4);
            break;
        }
        case (conf::game::Rest):
        {
            _velocity = sf::Vector2f(0, 0);
            break;
        }
        default:break;
    }
    if (new_direction != conf::game::Rest)
        _rotation = new_direction;
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


void GameObject::move(sf::Vector2f &shift)
{
    _position += shift;
}


void GameObject::set_velocity(sf::Vector2f &new_velocity)
{
    _velocity = new_velocity;
}


void GameObject::add_property(ser::Property *prop)
{
    _properties.push_back(prop);
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