#include "Bullet.h"

namespace cli
{

void Bullet::draw(sf::RenderWindow &window)
{
    window.draw(_texture);
}


Bullet::Bullet(sf::Vector2f &position) :
    GameObject(position, conf::game::Player)
{
    _texture.setSize(sf::Vector2f(10, 10));
    _texture.setFillColor(sf::Color::Yellow);
    _texture.setPosition(_position);
}

}