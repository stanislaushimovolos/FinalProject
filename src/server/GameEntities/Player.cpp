#include "Player.h"

namespace ser
{

Player::Player(Object &map_object) :
    ser::GameObject({map_object.rect.left, map_object.rect.top},
                    {0, 0},
                    conf::game::Up,
                    conf::game::Rest,
                    conf::game::player_speed,
                    conf::game::Player),

    // It is possible to use value of pointer as unique id
    _ptr_id(reinterpret_cast<std::uintptr_t>(this)),

    _shoot_clicks(0),
    _is_hit(false),
    _is_live(true),
    _health_points(conf::game::player_health_points)
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
    // Update position + animation
    GameObject::update(delta_t);

    auto main_sprite_ptr = dynamic_cast< MatrixSprite *>(_properties[conf::game::MainObjectSprite]);
    if (_is_hit)
    {
        if (!_is_live)
            main_sprite_ptr->set_texture_id(conf::render::GhostTexture);
        else
            main_sprite_ptr->set_color(sf::Color::Red);

    } else
        main_sprite_ptr->set_color(sf::Color::White);

    _is_hit = false;
}


void Player::interact(ser::GameObject *object, int delta_t)
{
    // Player can't interact with objects if he is dead:)
    auto other_type = object->get_type();
    if (!object->is_active() || !_is_live)
        return;

    // Don't do anything if there is no collision
    const auto &other_collider = object->get_collider();
    if (!this->_collider.detect_collision(other_collider))
        return;

    switch (other_type)
    {
        case (conf::game::Player) :
        {
            auto player_ptr = dynamic_cast<Player *>(object);
            if (!player_ptr->is_live())
                break;

            // Compute radius vector (unit) between 2 players
            auto other_position = object->get_position();
            sf::Vector2f radius_vector = compute_unit_vector(_position, other_position);

            // Don't move "reset" players
            if (_direction != conf::game::Rest)
                move(radius_vector * _speed * delta_t);

            if (object->get_direction() != conf::game::Rest)
                object->move(-radius_vector * _speed * delta_t);

            break;
        }
        case (conf::game::Bullet):
        {
            // Do not cause damage it his bullet
            auto bullet_ptr = dynamic_cast<Bullet *>(object);
            if (bullet_ptr->get_owner() != reinterpret_cast<std::uintptr_t>(this))
            {
                cause_damage(conf::game::bullet_damage);
                // Delete bullet after collision
                bullet_ptr->set_status(false);
            }

            break;
        }
        case (conf::game::MovingPlatform):
        {
            auto platform_ptr = dynamic_cast<MovingPlatform *>(object);
            cause_damage(platform_ptr->get_damage());

            auto other_position = object->get_position();
            sf::Vector2f radius_vector = compute_unit_vector(_position, other_position);
            move(radius_vector * _speed * delta_t);

            break;
        }
        case (conf::game::Blast):
        {
            auto blast_ptr = dynamic_cast<Blast *>(object);
            cause_damage(blast_ptr->get_damage());

            break;
        }
        case (conf::game::SolidBlock):
        {
            auto other_position = object->get_position();
            auto other_size = other_collider.get_size();

            auto this_size = _collider.get_size();
            auto this_position = _collider.get_position();

            auto other_bottom = other_position.y + other_size.y;
            auto other_right = other_position.x + other_size.x;
            auto other_left = other_position.x;
            auto other_top = other_position.y;

            auto this_bottom = this_position.y + this_size.y;
            auto this_right = this_position.x + this_size.x;
            auto this_left = this_position.x;
            auto this_top = this_position.y;

            // Check where is collision (works only if player collider "smaller" than solid block)
            if (this_top > other_top && this_bottom < other_bottom)
            {
                if (this_left < other_right && this_right >= other_right)
                    move({_speed * delta_t, 0});
                else if (this_right >= other_left && other_left >= this_left)
                    move({-_speed * delta_t, 0});
            }

            if (this_left > other_left && this_right < other_right)
            {
                if (this_bottom > other_top && this_top < other_top)
                    move({0, -_speed * delta_t});
                else if (this_top <= other_bottom && other_bottom <= this_bottom)
                    move({0, _speed * delta_t});
            }

            // TODO : Check cases when player collider "bigger" than solidblock

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
