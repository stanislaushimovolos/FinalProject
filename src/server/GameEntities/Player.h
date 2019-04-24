#pragma once
#include "../Engine/GameObject.h"

namespace ser
{

class Player : public ser::GameObject
{
 public:

    Player(sf::Vector2f start_position, sf::Vector2f start_velocity, float speed);

    Player();

    uint32_t get_direction() const;

    void set_direction(uint32_t new_direction);

    void update() override;


 private:
    uint32_t _direction;
};

}
