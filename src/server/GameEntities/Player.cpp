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
    _is_hit(false),
    _health_points(conf::game::player_health_points),
    _is_live(true)
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
                        boy_texture_height / 2});
    _collider.set_position({_position.x - boy_texture_width / 2,
                            _position.y - boy_texture_height / 4});
}


void Player::update(int delta_t)
{
    GameObject::update(delta_t);

    if (_is_hit)
    {
        if (!_is_live)
            dynamic_cast< MatrixSprite *>
            (_properties[conf::game::MainObjectSprite])->set_texture_id(conf::render::GhostTexture);
        else
            dynamic_cast< MatrixSprite *>
            (_properties[conf::game::MainObjectSprite])->set_color(sf::Color::Red);

    } else
        dynamic_cast< MatrixSprite *>
        (_properties[conf::game::MainObjectSprite])->set_color(sf::Color::White);

    _is_hit = false;
}


void Player::interact(ser::GameObject *object, int delta_t)
{
    auto other_type = object->get_type();
    if (!object->is_active() || !_is_live)
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

            std::cout << "Player-player collision!!!" << std::endl;
            auto other_position = object->get_position();
            sf::Vector2f radius_vector = compute_unit_vector(_position, other_position);

            if (_direction != conf::game::Rest)
                move(radius_vector * _speed * delta_t);

            if (object->get_direction() != conf::game::Rest)
                object->move(-radius_vector * _speed * delta_t);

            break;
        }
        case (conf::game::Bullet):
        {
            const auto &other_collider = object->get_collider();
            if (!this->_collider.detect_collision(other_collider))
                return;

            auto bullet_ptr = dynamic_cast<Bullet *>(object);
            if (bullet_ptr->get_owner() != reinterpret_cast<std::uintptr_t>(this))
            {
                std::cout << "Player-bullet collision!!!" << std::endl;
                cause_damage(conf::game::bullet_damage);
                bullet_ptr->set_status(false);
            }
            break;
        }
        case (conf::game::MovingPlatform):
        {
            const auto &other_collider = object->get_collider();
            if (!this->_collider.detect_collision(other_collider))
                return;

            auto platform_ptr = dynamic_cast<MovingPlatform *>(object);
            std::cout << "Player-hole collision!!!" << std::endl;
            cause_damage(platform_ptr->get_damage());

            auto other_position = object->get_position();
            sf::Vector2f radius_vector = compute_unit_vector(_position, other_position);

            move(radius_vector * _speed * delta_t);

            break;
        }
        case (conf::game::Blast):
        {
            const auto &other_collider = object->get_collider();
            if (!this->_collider.detect_collision(other_collider))
                return;

            auto blast_ptr = dynamic_cast<Blast *>(object);
            std::cout << "Player-blast collision!!!" << std::endl;
            cause_damage(blast_ptr->get_damage());

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


void Player::cause_damage(float damage)
{
    _is_hit = true;
    _health_points -= damage;
    if (_health_points <= 0)
        _is_live = false;
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


bool Player::is_live() const
{
    return _is_live;
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
