#pragma once
#include "../Engine/GameObject.h"

namespace ser
{

class Player : public ser::GameObject
{
 public:
    Player(sf::Vector2f start_position, sf::Vector2f start_velocity, float speed);

    Player();

    sf::Vector2f get_position()
    {
        return _position;
    }
};

}
