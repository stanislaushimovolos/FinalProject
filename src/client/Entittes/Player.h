#pragma once

#include "../Engine/GameObject.h"

namespace cli
{

class Player : public GameObject
{
 public:
    void draw(sf::RenderWindow &window) override;

    explicit Player(sf::Vector2f pos);

 private:
    sf::RectangleShape _texture;
};

}
