#include "Bullet.h"

namespace cli
{

void Bullet::draw(sf::RenderWindow &window)
{
    window.draw(_texture);
}


Bullet::Bullet() :
    GameObject(sf::Vector2f(0, 0), conf::game::Bullet)
{
    _texture.setSize(sf::Vector2f(10, 10));
    _texture.setFillColor(sf::Color::Cyan);
    _texture.setPosition(_position);
}


Bullet::Bullet(sf::Packet &pack) :
    Bullet()
{
    set_state_form_packet(pack);
}


void Bullet::set_state_form_packet(sf::Packet &pack)
{
    float x, y = 0;
    pack >> x >> y;

    set_position({x, y});
    _texture.setPosition({x, y});
}

}