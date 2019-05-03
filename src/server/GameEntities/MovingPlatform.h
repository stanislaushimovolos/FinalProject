#pragma once

#include "Player.h"
#include "../Engine/GameObject.h"
#include "../../configuration/render.h"

namespace ser
{
class MovingPlatform : public GameObject
{
 public:

    MovingPlatform(Object &map_object,
                   float damage);

    void compress_to_packet(sf::Packet &pack) const override;

    void interact(GameObject *object, int delta_t) override;

    void update(int delta_t) override;

    float get_damage();

    ~MovingPlatform() override = default;

 private:

    int _time_of_movement;
    float _restart_movement_time;

    float _caused_damage;
};

}