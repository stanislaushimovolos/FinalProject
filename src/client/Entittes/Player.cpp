//
// Created by superstraz on 4/23/19.
//

#include "Player.h"


namespace cli
{

void Player::draw(sf::RenderWindow &window)
{
    window.draw(_texture);
}


Player::Player(sf::Vector2f& position) :
    GameObject(position, config::Player)
{
    _texture.setSize(sf::Vector2f(40, 40));
    _texture.setFillColor(sf::Color::Red);
    _texture.setPosition(_position);
}

}