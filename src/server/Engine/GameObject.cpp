#include "GameObject.h"

namespace ser
{

GameObject::GameObject(sf::Vector2f pos,
                       sf::Vector2f velocity,
                       uint32_t start_rotation,
                       uint32_t start_direction,
                       float speed,
                       uint32_t type) :
    _position(pos),
    _velocity(velocity),
    _speed(speed),
    _rotation(start_rotation),
    _direction(start_direction),
    _type(type),
    _is_active(true),
    _collider()
{}


void GameObject::set_speed_from_direction(uint32_t new_direction)
{
    _direction = new_direction;
    switch (new_direction)
    {
        case (conf::game::Left):
        {
            _velocity = sf::Vector2f(-_speed, 0);
            break;
        }
        case (conf::game::Right):
        {
            _velocity = sf::Vector2f(_speed, 0);
            break;
        }
        case (conf::game::Up):
        {
            _velocity = sf::Vector2f(0, -_speed);
            break;
        }
        case (conf::game::Down):
        {
            _velocity = sf::Vector2f(0, _speed);
            break;
        }
        case (conf::game::DownRight):
        {
            // sqrt(2)
            _velocity = sf::Vector2f(_speed / 1.4, _speed / 1.4);
            break;
        }
        case (conf::game::UpRight):
        {
            // sqrt(2)
            _velocity = sf::Vector2f(_speed / 1.4, -_speed / 1.4);
            break;
        }
        case (conf::game::DownLeft):
        {
            // sqrt(2)
            _velocity = sf::Vector2f(-_speed / 1.4, _speed / 1.4);
            break;
        }
        case (conf::game::UpLeft):
        {
            // sqrt(2)
            _velocity = sf::Vector2f(-_speed / 1.4, -_speed / 1.4);
            break;
        }
        case (conf::game::Rest):
        {
            _velocity = sf::Vector2f(0, 0);
            break;
        }
        default:break;
    }
    if (new_direction != conf::game::Rest)
        _rotation = new_direction;
}


void GameObject::update(int delta_t)
{
    move({_velocity.x * delta_t, _velocity.y * delta_t});
    _collider.set_position(_position);

    for (auto &[_, prop]:_properties)
        prop->update(delta_t);
}


void GameObject::set_direction(uint32_t new_direction)
{
    _direction = new_direction;
    switch (new_direction)
    {
        default:break;
    }

    if (new_direction != conf::game::Rest)
        _rotation = new_direction;
}


void GameObject::compress_properties_to_packet(sf::Packet &pack) const
{
    for (auto &[_, prop]:_properties)
    {
        pack << prop->get_type();
        prop->compress_to_packet(pack);
    }
}


void GameObject::move(sf::Vector2f &&shift)
{
    _position += shift;
}


void GameObject::set_velocity(sf::Vector2f &new_velocity)
{
    _velocity = new_velocity;
}


void GameObject::add_property(uint32_t key, GraphProperty *prop)
{
    _properties[key] = prop;
}


void GameObject::set_status(bool status)
{
    _is_active = status;
}


bool GameObject::is_active() const
{
    return _is_active;
}


uint32_t GameObject::get_direction() const
{
    return _direction;
}


uint32_t GameObject::get_rotation() const
{
    return _rotation;
}


uint32_t GameObject::get_type() const
{
    return _type;
}


sf::Vector2f GameObject::get_position() const
{
    return _position;
}


sf::Vector2f GameObject::get_velocity() const
{
    return _position;
}


const RectCollider &GameObject::get_collider() const
{
    return _collider;
}


sf::Vector2f compute_unit_vector(const sf::Vector2f &first, const sf::Vector2f &second)
{
    auto delta_x = first.x - second.x;
    auto delta_y = first.y - second.y;

    sf::Vector2f radius_vector(delta_x, delta_y);

    float vector_norm = fast_square_root((delta_x) * (delta_x) + (delta_y) * (delta_y));

    // normalize
    radius_vector.x = radius_vector.x / vector_norm;
    radius_vector.y = radius_vector.y / vector_norm;

    return {radius_vector.x, radius_vector.y};
}


sf::Vector2f operator*(const sf::Vector2f &first, float multiplier)
{
    return {first.x * multiplier, first.y * multiplier};
}


float fast_square_root(float n)
{
    n = 1.0f / n;
    long i;
    float x, y;

    x = n * 0.5f;
    y = n;
    i = *(long *) &y;
    i = 0x5f3759df - (i >> 1);
    y = *(float *) &i;
    y = y * (1.5f - (x * y * y));

    return y;
}

}