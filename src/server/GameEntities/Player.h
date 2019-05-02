#pragma once

#include "Blast.h"
#include "Bullet.h"
#include "MovingPlatform.h"
#include "../Engine/GameObject.h"
#include "../../configuration/render.h"

namespace ser
{

class Player : public GameObject
{
 public:

    explicit Player(std::pair<uint32_t, uint32_t> ip_port);

    void compress_to_packet(sf::Packet &pack) const override;

    void interact(GameObject *object, int delta_t) override;

    void update(int delta_t) override;

    void set_id(uint64_t id);

    void cause_damage(float damage);

    bool add_shoot_click(bool is_shoot);

    bool is_live() const;

    uint64_t get_id() const;

    ~Player() override = default;

 private:

    uint32_t _ip;
    uint32_t _port;
    uintptr_t _ptr_id;

    bool _is_hit;
    uint32_t _shoot_clicks;
    float _health_points;

    bool _is_live;
};

std::pair<uint32_t, uint32_t> split_long_long(uint64_t number);

sf::Packet &operator<<(sf::Packet &packet, uint64_t number);

}
