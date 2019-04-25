#pragma once

#include "../Engine/GameObject.h"

namespace cli
{

class Player : public GameObject
{
 public:
    void set_state_form_packet(sf::Packet &pack) override;

    void draw(sf::RenderWindow &window) override;

    explicit Player();

 private:
    sf::RectangleShape _texture;
    uint32_t _ip;
    uint32_t _port;
};

}
