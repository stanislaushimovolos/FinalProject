#include "Player.h"
#include "../../configuration/config.h"

namespace ser
{

Player::Player(sf::Vector2f start_position, sf::Vector2f start_velocity, float speed) :
    ser::GameObject(start_position, start_velocity, speed),
    _direction(conf::game::Rest)
{}


Player::Player() :
    ser::GameObject({0, 0}, {0, 0}, 30),
    _direction(conf::game::Rest)
{}


uint32_t Player::get_direction() const
{
    return _direction;
}


void Player::set_direction(uint32_t new_direction)
{
    _direction = new_direction;
}


void Player::to_packet(sf::Packet &pack) const
{
    pack << _position.x << _position.y;
}


void Player::update()
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

}