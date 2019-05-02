#include "MovingPlatform.h"

namespace ser
{
MovingPlatform::MovingPlatform(sf::Vector2f position,
                               sf::Vector2f velocity,
                               int time_to_one_side,
                               float damage) :
    GameObject(position,
               velocity,
               conf::game::Down,
               conf::game::Down,
               0,
               conf::game::MovingPlatform),

    _time_of_movement(0),
    _restart_movement_time(time_to_one_side),
    _caused_damage(damage)
{
    using namespace conf::render;

    add_property(conf::game::MainObjectSprite,
                 new MatrixSprite(this,
                                  BlackHoleTexture,
                                  hole_animation_speed,
                                  hole_texture_width,
                                  hole_texture_height,
                                  hole_frame_amount));


    _collider.set_size({hole_texture_width,
                        hole_texture_height});
    _collider.set_position({_position.x - blast_texture_width / 2,
                            _position.y - blast_texture_height / 2});

}


void MovingPlatform::compress_to_packet(sf::Packet &pack) const
{
    pack << _position.x << _position.y << (uint32_t) _properties.size();
    compress_properties_to_packet(pack);
}


void MovingPlatform::interact(ser::GameObject *object, int delta_t)
{
    auto other_type = object->get_type();
    if (!object->is_active())
        return;

    switch (other_type)
    {
        case (conf::game::Player) :
        {
            const auto &other_collider = object->get_collider();
            if (!this->_collider.detect_collision(other_collider))
                return;

            auto player_ptr = dynamic_cast<Player *>(object);
            if (!player_ptr->is_live())
                break;

            player_ptr->cause_damage(_caused_damage);

            auto other_position = object->get_position();
            sf::Vector2f radius_vector = compute_unit_vector(_position, other_position);

            move(radius_vector * _speed * delta_t);
            break;
        }
        default:break;
    }

}


void MovingPlatform::update(int delta_t)
{
    GameObject::update(delta_t);
    _time_of_movement += delta_t;
    if (_time_of_movement >= _restart_movement_time)
    {
        _velocity.x = -_velocity.x;
        _velocity.y = -_velocity.y;
        _time_of_movement = 0;
    }
}


float MovingPlatform::get_damage()
{
    return _caused_damage;
}

}