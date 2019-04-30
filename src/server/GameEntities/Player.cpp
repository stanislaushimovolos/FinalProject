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
    ser::GameObject({0, 0},
                    {0, 0},
                    conf::game::Up,
                    conf::game::Rest,
                    conf::game::player_speed,
                    conf::game::Player),
    _ip(ip_port.first),
    _port(ip_port.second),
    _shoot_clicks(0)
{
    add_property(new MatrixSprite(this,
                                  conf::game::BoyTexture,
                                  conf::game::boy_animation_speed,
                                  conf::game::boy_texture_width,
                                  conf::game::boy_texture_height,
                                  conf::game::boy_frame_amount));
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
