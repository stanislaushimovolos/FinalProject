#include "Bullet.h"

namespace ser
{

Bullet::Bullet(sf::Vector2f position, uint32_t player_rotation) :
    ser::GameObject(position,
                    {0, 0},
                    player_rotation,
                    player_rotation,
                    conf::game::bullet_speed,
                    conf::game::Bullet)
{
    set_speed_from_direction(player_rotation);
    add_property(new MatrixSprite(this,
                                  conf::game::BulletTexture,
                                  conf::game::bullet_animation_speed,
                                  conf::game::bullet_texture_width,
                                  conf::game::bullet_texture_height,
                                  conf::game::bullet_frame_amount));
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