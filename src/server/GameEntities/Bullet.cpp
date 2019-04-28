#include "Bullet.h"

namespace ser
{

Bullet::Bullet(sf::Vector2f position, uint32_t direction) :
    ser::GameObject(position, {0, 0}, direction, direction, 5, conf::game::Bullet)
{
    set_direction(direction);
    add_property(new SimpleRectangleTexture(this, sf::Color::Magenta, {7, 7}));
}


void Bullet::set_direction(uint32_t new_direction)
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
        default:break;
    }

    if (new_direction != conf::game::Rest)
        _rotation = new_direction;

}


void Bullet::update(int delta_t)
{
    auto delta_r = sf::Vector2f(_velocity.x * delta_t, _velocity.y * delta_t);
    move(delta_r);

    for (auto &prop:_properties)
        prop->update(delta_t);
}


void Bullet::compress_to_packet(sf::Packet &pack) const
{
    pack << _position.x << _position.y << (uint32_t) _properties.size();
    for (auto &prop:_properties)
    {
        pack << prop->get_type();
        prop->compress_to_packet(pack);
    }
}

}