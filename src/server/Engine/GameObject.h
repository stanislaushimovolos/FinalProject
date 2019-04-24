#pragma once

#include <SFML/System.hpp>
namespace ser
{

// abstract class for all game entities
class GameObject
{
 public:
    GameObject() = delete;

    explicit GameObject(sf::Vector2f pos,
                        sf::Vector2f velocity,
                        float speed);

    sf::Vector2f get_position() const;

    void move(sf::Vector2f &&shift);

    virtual void update() = 0;

 protected:
    sf::Vector2f _position;
    sf::Vector2f _velocity;
    float _speed;

};

}