#pragma once

#include <cmath>

#include "../Engine/GameObject.h"
#include "../../configuration/config.h"

namespace ser
{

class Player : public GameObject
{
 public:

    Player(sf::Vector2f start_position, float speed);

    explicit Player(std::pair<uint32_t, uint32_t> ip_port);

    void compress_to_packet(sf::Packet &pack) const override;

    void interact(GameObject *object, int delta_t) override;

    void update(int delta_t) override;

    ~Player() override = default;

    uint32_t _shoot_clicks;

 private:

    uint32_t _ip;
    uint32_t _port;
};

}
