#pragma once

#include "Player.h"
#include "../Engine/GameObject.h"
#include "../../configuration/render.h"

namespace ser
{

class Bullet : public GameObject
{
 public:

    Bullet(uint64_t owner, sf::Vector2f position, uint32_t player_rotation);

    void compress_to_packet(sf::Packet &pack) const override;

    void interact(GameObject *object, int delta_t) override;

    float get_damage() const;

    uint64_t get_owner();

    ~Bullet() override = default;

 private:

    float _caused_damage;
    uint64_t _owner;
};

}