#pragma once
#include "../Engine/GameObject.h"

namespace cli
{

class Bullet : public GameObject
{
 public:
    void draw(sf::RenderWindow &window) override;

    explicit Bullet(sf::Vector2f &pos);

 private:
    sf::RectangleShape _texture;
};

}