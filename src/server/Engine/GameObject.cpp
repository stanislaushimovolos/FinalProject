//
// Created by superstraz on 4/22/19.
//

#include "GameObject.h"

namespace ser
{
GameObject::GameObject(sf::Vector2f pos,
                       sf::Vector2f velocity,
                       float speed) :
    _position(pos),
    _velocity(velocity),
    _speed(speed)
{}


void GameObject::move(sf::Vector2f shift)
{
    _position += shift;
}


sf::Vector2f GameObject::get_position() const
{
    return _position;
}

}