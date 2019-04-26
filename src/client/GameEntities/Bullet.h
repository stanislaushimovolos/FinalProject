#pragma once
#include "../Engine/GameObject.h"

namespace cli
{

class Bullet : public GameObject
{
 public:

    explicit Bullet(sf::Packet &pack);

    explicit Bullet();

    void set_state_form_packet(sf::Packet &pack) override;

    void draw(sf::RenderWindow &window) override;

 private:
    sf::RectangleShape _texture;
};

}