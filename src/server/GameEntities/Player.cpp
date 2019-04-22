#include "Player.h"

namespace ser
{

Player::Player(sf::Vector2f start_position, sf::Vector2f start_velocity, float speed) :
    ser::GameObject(start_position, start_velocity, speed)
{}


Player::Player() :
    ser::GameObject({0, 0}, {0, 0}, 10)
{}

}