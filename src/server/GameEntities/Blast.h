#pragma once

#include "../Engine/GameObject.h"
#include "../../configuration/render.h"

#include "Player.h"

namespace ser
{

class Blast : public GameObject
{

 public:

    Blast(uint64_t owner, sf::Vector2f position, float damage);

    void compress_to_packet(sf::Packet &pack) const override;

    void interact(GameObject *object, int delta_t) override;

    void update(int delta_t) override;

    float get_damage() const;

    ~Blast() override = default;

 private:

    uint64_t _owner;
    float _life_time;
    float _caused_damage;

};

}