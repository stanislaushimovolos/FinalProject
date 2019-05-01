#include "RectCollider.h"

namespace ser
{

RectCollider::RectCollider(sf::Vector2f &position, sf::Vector2f &size) :
    RectCollider()
{
    _bounding_box.setSize(size);
    _bounding_box.setPosition(position);
    _bounding_box.setFillColor(sf::Color::Transparent);
}


bool RectCollider::detect_collision(const RectCollider &other_collider) const
{
    auto other_size = other_collider.get_size();
    auto other_position = other_collider.get_position();

    auto this_size = this->get_size();
    auto this_position = this->get_position();

    auto other_bottom = other_position.y + other_size.y;
    auto other_right = other_position.x + other_size.x;
    auto other_left = other_position.x;
    auto other_top = other_position.y + other_size.y / 2;

    auto this_bottom = this_position.y + this_size.y;
    auto this_right = this_position.x + this_size.x;
    auto this_left = this_position.x;
    auto this_top = this_position.y + this_size.y / 2;

    if (this_left > other_right ||
        this_right < other_left ||
        this_top > other_bottom ||
        this_bottom < other_top)
        return false;

    return true;
}


void RectCollider::set_position(const sf::Vector2f &new_position)
{
    _bounding_box.setPosition(new_position);
}


void RectCollider::set_size(const sf::Vector2f &new_size)
{
    _bounding_box.setSize(new_size);
}


sf::Vector2f RectCollider::get_position() const
{
    return _bounding_box.getPosition();
}


sf::Vector2f RectCollider::get_size() const
{
    return _bounding_box.getSize();
}

}
