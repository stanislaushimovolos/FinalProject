#pragma once
#include "../Engine/GameObject.h"
#include "../../configuration/config.h"

namespace ser
{

class Bullet : public GameObject
{
 public:

    explicit Bullet(sf::Vector2f position, uint32_t direction);

    void to_packet(sf::Packet &pack) const override;

    void update() override;

    uint32_t get_direction() const;

    void set_direction(uint32_t new_direction);

    ~Bullet() override = default;

 private:

    uint32_t _direction;

};

}