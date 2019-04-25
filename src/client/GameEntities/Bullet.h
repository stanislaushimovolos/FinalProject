#pragma once
#include "../Engine/GameObject.h"

namespace cli
{

class Bullet : public GameObject
{
 public:

    void set_state_form_packet(sf::Packet &pack) override;

    void draw(sf::RenderWindow &window) override;

    explicit Bullet();

 private:
    sf::RectangleShape _texture;
};

}