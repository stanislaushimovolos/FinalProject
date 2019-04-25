#pragma once
#include "../Engine/GameObject.h"
#include "../../configuration/config.h"

namespace ser
{

class Bullet : public GameObject
{
 public:

    explicit Bullet(sf::Vector2f position, uint32_t direction);

    void set_direction(uint32_t new_direction) override;

    void compress_to_packet(sf::Packet &pack) const override;

    void update() override;

    ~Bullet() override = default;
};

}