//
// Created by superstraz on 4/23/19.
//

#include "GameObject.h"

namespace cli
{

GameObject::GameObject() :
    _position(0, 0),
    _type(config::Empty)
{}


GameObject::GameObject(sf::Vector2f pos, uint32_t type) :
    _position(pos),
    _type(type)
{}


void GameObject::set_position(sf::Vector2f pos)
{
    _position = pos;
}


sf::Vector2f GameObject::get_position() const
{
    return _position;
}

}
