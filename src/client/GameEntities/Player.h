#pragma once

#include "../Engine/GameObject.h"

namespace cli
{

class Player : public GameObject
{
 public:

    explicit Player(sf::Packet &pack);

    explicit Player();

    void set_state_form_packet(sf::Packet &pack) override;

    void draw(sf::RenderWindow &window) override;

    std::pair<uint32_t, uint32_t> get_id() const;


 private:
    sf::RectangleShape _texture;
    uint32_t _ip;
    uint32_t _port;
};

}
