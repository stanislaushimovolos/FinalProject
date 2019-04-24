#include "Bullet.h"

namespace ser
{

Bullet::Bullet(sf::Vector2f position, uint32_t direction) :
    ser::GameObject(position, {0, 0}, direction, direction, 20, conf::game::Bullet)
{
    set_direction(direction);
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


void Bullet::update()
{
    move(_velocity);
}


void Bullet::to_packet(sf::Packet &pack) const
{
    pack << _position.x << _position.y;
}


}