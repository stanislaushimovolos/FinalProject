#pragma once
#include "../Engine/GameObject.h"
#include "../../configuration/config.h"

namespace ser
{

class Bullet : public GameObject
{
 public:

    explicit Bullet(sf::Vector2f position, uint32_t player_rotation);

    void compress_to_packet(sf::Packet &pack) const override;

    void update(int delta_t) override;

    ~Bullet() override = default;
};

}