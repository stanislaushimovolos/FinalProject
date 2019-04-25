#include "Player.h"


namespace cli
{

void Player::draw(sf::RenderWindow &window)
{
    window.draw(_texture);
}


Player::Player() :
    GameObject(sf::Vector2f(0, 0), conf::game::Player)
{
    _texture.setSize(sf::Vector2f(40, 40));
    _texture.setFillColor(sf::Color::Red);
    _texture.setPosition(_position);
}


void Player::set_state_form_packet(sf::Packet &pack)
{
    float x, y = 0;
    uint32_t ip = 0, port = 0;
    pack >> ip >> port >> x >> y;

    set_position({x, y});
    _texture.setPosition({x, y});

    _port = port;
    _ip = ip;
}

}