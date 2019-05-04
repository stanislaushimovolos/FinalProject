#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

namespace ser
{

class RectangleCollider
{
 public:

    RectangleCollider(sf::Vector2f &position, sf::Vector2f &size);

    RectangleCollider() = default;

    bool detect_collision(const RectangleCollider &other_collider) const;

    void set_position(const sf::Vector2f &new_position);

    void set_size(const sf::Vector2f &new_size);

    sf::Vector2f get_position() const;

    sf::Vector2f get_size() const;

 private:

    sf::RectangleShape _bounding_box;
};

}

