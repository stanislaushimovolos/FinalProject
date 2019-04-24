#include "Bullet.h"

namespace ser
{

Bullet::Bullet(sf::Vector2f position, uint32_t direction) :
    ser::GameObject(position, {0, 0}, 70, conf::game::Bullet),
    _direction(direction)
{}


void Bullet::update()
{
    switch (_direction)
    {
        case (conf::game::Left):
        {
            move(sf::Vector2f(-_speed, 0));
            break;
        }
        case (conf::game::Right):
        {
            move(sf::Vector2f(_speed, 0));
            break;
        }
        case (conf::game::Up):
        {
            move(sf::Vector2f(0, -_speed));
            break;
        }
        case (conf::game::Down):
        {
            move(sf::Vector2f(0, _speed));
            break;
        }
        default:break;
    }
}


uint32_t Bullet::get_direction() const
{
    return _direction;
}


void Bullet::set_direction(uint32_t new_direction)
{
    _direction = new_direction;
}


void Bullet::to_packet(sf::Packet &pack) const
{
    pack << _position.x << _position.y;
}


}