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
    pack << _ptr_id << _position.x << _position.y << (uint32_t) _properties.size();
    compress_properties_to_packet(pack);
}


void Player::set_id(uint64_t id)
{
    _ptr_id = id;
}


bool Player::add_shoot_click(bool is_shoot)
{
    if (is_shoot)
    {
        if (_shoot_clicks < conf::game::one_shoot_required_clicks)
        {
            _shoot_clicks++;
            if (_shoot_clicks == 1)
                return true;
        } else
            _shoot_clicks = 0;
        return false;
    }

    _shoot_clicks = 0;
    return false;
}


uint64_t Player::get_id() const
{
    return _ptr_id;
}


std::pair<uint32_t, uint32_t> split_long_long(uint64_t number)
{
    auto first_part = (uint32_t) (number & 0xFFFFFFFF);
    auto second_part = (uint32_t) (number >> 8 * sizeof(int));
    return std::make_pair(first_part, second_part);
}


sf::Packet &operator<<(sf::Packet &packet, uint64_t number)
{
    auto[first_bits, last_bits] = split_long_long(number);
    return packet << first_bits << last_bits;
}

}
