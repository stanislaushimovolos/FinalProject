#include "Bullet.h"

namespace ser
{

Bullet::Bullet(uint64_t owner, sf::Vector2f position, uint32_t player_rotation) :
    ser::GameObject(position,
                    {0, 0},
                    player_rotation,
                    player_rotation,
                    conf::game::bullet_speed,
                    conf::game::Bullet),

    _owner(owner),
    _caused_damage(conf::game::bullet_damage)
{
    using namespace conf::render;

    set_speed_from_direction(player_rotation);
    add_property(conf::game::MainObjectSprite,
                 new MatrixSprite(this,
                                  BulletTexture,
                                  bullet_animation_speed,
                                  bullet_texture_width,
                                  bullet_texture_height,
                                  bullet_frame_amount));

    _collider.set_size({bullet_texture_width,
                        bullet_texture_height});
    _collider.set_position({_position.x - bullet_texture_width / 2,
                            _position.y - bullet_texture_height / 2});
}


void Bullet::interact(ser::GameObject *object, int delta_t)
{
    auto other_type = object->get_type();
    if (!object->is_active())
        return;

    switch (other_type)
    {
        case (conf::game::Player):
        {
            const auto &other_collider = object->get_collider();
            if (!this->_collider.detect_collision(other_collider))
                return;

            auto player_ptr = dynamic_cast<Player *>(object);
            if (player_ptr->is_active())
                player_ptr->cause_damage(_caused_damage);


            set_status(false);
            break;
        }
        case (conf::game::SolidBlock):
        {
            const auto &other_collider = object->get_collider();
            if (!this->_collider.detect_collision(other_collider))
                return;

            set_status(false);
            break;
        }
        default:break;
    }
}


void Bullet::compress_to_packet(sf::Packet &pack) const
{
    pack << _position.x << _position.y << (uint32_t) _properties.size();
    compress_properties_to_packet(pack);
}


uint64_t Bullet::get_owner()
{
    return _owner;
}


float Bullet::get_damage() const
{
    return _caused_damage;
}


}