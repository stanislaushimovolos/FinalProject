#include "Player.h"

namespace ser
{

Player::Player(std::pair<uint32_t, uint32_t> ip_port) :
    ser::GameObject({0, 0},
                    {0, 0},
                    conf::game::Up,
                    conf::game::Rest,
                    conf::game::player_speed,
                    conf::game::Player),

    _ptr_id(reinterpret_cast<std::uintptr_t>(this)),
    _ip(ip_port.first),
    _port(ip_port.second),
    _shoot_clicks(0),
    _is_hit(false)
{
    using namespace conf::render;

    add_property(conf::game::MainObjectSprite,
                 new MatrixSprite(this,
                                  BoyTexture,
                                  boy_animation_speed,
                                  boy_texture_width,
                                  boy_texture_height,
                                  boy_frame_amount));

    _collider.set_size({boy_texture_width / 2,
                        boy_texture_height});
    _collider.set_position({_position.x - boy_texture_width / 2,
                            _position.y - -boy_texture_height / 2});
}


void Player::update(int delta_t)
{
    GameObject::update(delta_t);
    if (_is_hit)
        dynamic_cast< MatrixSprite *>
        (_properties[conf::game::MainObjectSprite])->set_color(sf::Color::Red);
    else
        dynamic_cast< MatrixSprite *>
        (_properties[conf::game::MainObjectSprite])->set_color(sf::Color::White);

    _is_hit = false;
}


void Player::interact(ser::GameObject *object, int delta_t)
{
    auto other_type = object->get_type();

    switch (other_type)
    {
        case (conf::game::Player) :
        {
            const auto &other_collider = object->get_collider();
            if (this->_collider.detect_collision(other_collider))
            {
                auto other_position = object->get_position();
                sf::Vector2f
                    radius_vector
                    (_position.x - other_position.x, _position.y - other_position.y);

                float vector_norm = fast_square_root(
                    (_position.x - other_position.x) * (_position.x - other_position.x)
                        + (_position.y - other_position.y) * (_position.y - other_position.y));

                // normalize
                radius_vector.x = radius_vector.x / vector_norm;
                radius_vector.y = radius_vector.y / vector_norm;

                if (_direction != conf::game::Rest)
                    move({radius_vector.x * _speed * delta_t,
                          radius_vector.y * _speed * delta_t});

                if (object->get_direction() != conf::game::Rest)
                    object->move({-radius_vector.x * _speed * delta_t,
                                  -radius_vector.y * _speed * delta_t});
            }
            break;
        }
        case (conf::game::Bullet):
        {
            const auto &other_collider = object->get_collider();
            if (this->_collider.detect_collision(other_collider))
            {
                auto bullet_ptr = dynamic_cast<Bullet *>(object);
                if (bullet_ptr->get_owner() != reinterpret_cast<std::uintptr_t>(this))
                    _is_hit = true;
            }
            break;
        }
        default:break;
    }
}


void Player::compress_to_packet(sf::Packet &pack) const
{
    pack << _ip << _port << _position.x << _position.y << (uint32_t) _properties.size();
    for (auto &[_, prop]:_properties)
    {
        pack << prop->get_type();
        prop->compress_to_packet(pack);
    }
}


void Player::set_id(uint64_t id)
{
    _ptr_id = id;
}


uint64_t Player::get_id() const
{
    return _ptr_id;
}

}
