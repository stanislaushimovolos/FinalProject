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
    add_property(0,
                 new MatrixSprite(this,
                                  conf::game::BulletTexture,
                                  conf::game::bullet_animation_speed,
                                  conf::game::bullet_texture_width,
                                  conf::game::bullet_texture_height,
                                  conf::game::bullet_frame_amount));

    _collider.set_size({conf::game::bullet_texture_width,
                        conf::game::bullet_texture_height});
    _collider.set_position({_position.x - conf::game::bullet_texture_width / 2,
                            _position.y - -conf::game::bullet_texture_height / 2});
}


void Bullet::interact(ser::GameObject *object, int delta_t)
{
    auto other_type = object->get_type();
    switch (other_type)
    {
        case (conf::game::Bullet) :
        {
            break;
        }
        default:break;
    }
}


void Bullet::compress_to_packet(sf::Packet &pack) const
{
    pack << _position.x << _position.y << (uint32_t) _properties.size();
    for (auto &[_, prop]:_properties)
    {
        pack << prop->get_type();
        prop->compress_to_packet(pack);
    }
}

}