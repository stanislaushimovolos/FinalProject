#pragma once

#include "../Engine/GameObject.h"
#include "../../configuration/render.h"

namespace ser
{
class MovingPlatform : public GameObject
{
 public:

    MovingPlatform(sf::Vector2f position, sf::Vector2f velocity, int time_to_one_side);

    void compress_to_packet(sf::Packet &pack) const override;

    void interact(GameObject *object, int delta_t) override;

    void update(int delta_t) override;

    ~MovingPlatform() override = default;

 private:

    int _time_of_movement;
    int _restart_movement_time;
};

}