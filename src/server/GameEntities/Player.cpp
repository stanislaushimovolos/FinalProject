#include "Player.h"

namespace ser
{

Player::Player(sf::Vector2f start_position, float speed) :
    ser::GameObject(start_position,
                    {0, 0},
                    conf::game::Up,
                    conf::game::Rest,
                    speed,
                    conf::game::Player)
{}


Player::Player(std::pair<uint32_t, uint32_t> ip_port) :
    ser::GameObject({0, 0}, {0, 0}, conf::game::Up, conf::game::Rest, 1, conf::game::Player),
    _ip(ip_port.first),
    _port(ip_port.second)
{
    add_property(new MatrixSprite(this,
                                  conf::game::DudeTexture,
                                  conf::game::dude_animation_speed,
                                  conf::game::dude_width,
                                  conf::game::dude_height,
                                  conf::game::dude_frame_amount));
}


void Player::set_direction(uint32_t new_direction)
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


void Player::compress_to_packet(sf::Packet &pack) const
{
    pack << _ip << _port << _position.x << _position.y << (uint32_t) _properties.size();
    for (auto &prop:_properties)
    {
        pack << prop->get_type();
        prop->compress_to_packet(pack);
    }
}


void Player::update(int delta_t)
{
    auto delta_r = sf::Vector2f(_velocity.x * delta_t, _velocity.y * delta_t);
    move(delta_r);

    for (auto &prop:_properties)
        prop->update(delta_t);
}

}
