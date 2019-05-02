#include "MovingPlatform.h"

namespace ser
{
MovingPlatform::MovingPlatform(sf::Vector2f position, sf::Vector2f velocity, int time_to_one_side) :
    GameObject(position,
               velocity,
               conf::game::Down,
               conf::game::Down,
               0,
               conf::game::MovingPlatform),

    _time_of_movement(0),
    _restart_movement_time(time_to_one_side),
    _caused_damage(conf::game::hole_damage)
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

}