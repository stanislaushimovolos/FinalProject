#include "RectangleCollider.h"

namespace ser
{

RectangleCollider::RectangleCollider(sf::Vector2f &position, sf::Vector2f &size) :
    RectangleCollider()
{
    _bounding_box.setSize(size);
    _bounding_box.setPosition(position);
    _bounding_box.setFillColor(sf::Color::Transparent);
}


bool RectangleCollider::detect_collision(const RectangleCollider &other_collider) const
{
    sf::Vector2f other_size = other_collider.get_size();
    sf::Vector2f other_position = other_collider.get_position();

    sf::Vector2f this_size = this->get_size();
    sf::Vector2f this_position = this->get_position();

    float other_bottom = other_position.y + other_size.y;
    float other_right = other_position.x + other_size.x;
    float other_left = other_position.x;
    float other_top = other_position.y;

    float this_bottom = this_position.y + this_size.y;
    float this_right = this_position.x + this_size.x;
    float this_left = this_position.x;
    float this_top = this_position.y;

    // It's much easier to check if there is no collision
    if (this_left > other_right ||
        this_right < other_left ||
        this_top > other_bottom ||
        this_bottom < other_top)
        return false;

    return true;
}


void RectangleCollider::set_position(const sf::Vector2f &new_position)
{
    _bounding_box.setPosition(new_position);
}


void RectangleCollider::set_size(const sf::Vector2f &new_size)
{
    _bounding_box.setSize(new_size);
}


sf::Vector2f RectangleCollider::get_position() const
{
    return _bounding_box.getPosition();
}


sf::Vector2f RectangleCollider::get_size() const
{
    return _bounding_box.getSize();
}

}
