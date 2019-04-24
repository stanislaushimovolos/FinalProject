#pragma once
#include "../Engine/GameObject.h"
#include "../../configuration/config.h"

namespace ser
{

class Player : public GameObject
{
 public:

    Player(sf::Vector2f start_position, float speed);

    Player();

    void to_packet(sf::Packet &pack) const override;

    void set_direction(uint32_t new_direction) override;

    void update() override;

    ~Player() override = default;

};

}
