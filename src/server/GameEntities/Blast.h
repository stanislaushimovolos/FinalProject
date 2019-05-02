#pragma once

#include "../Engine/GameObject.h"
#include "../../configuration/render.h"

namespace ser
{

class Blast : public GameObject
{

 public:

    Blast(uint64_t owner, sf::Vector2f position);

    void compress_to_packet(sf::Packet &pack) const override;

    void interact(GameObject *object, int delta_t) override;

    void update(int delta_t) override;

    ~Blast() override = default;

 private:

    uint64_t _owner;
    float _life_time;

};

}