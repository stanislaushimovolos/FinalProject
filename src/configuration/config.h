#pragma once
#include <iostream>
#include <SFML/Network.hpp>

namespace config
{
static const uint32_t REQUIRED_NUM_OF_CLIENTS = 2;
static const uint32_t CONNECTION_DELAY = 20;

static const sf::IpAddress remote_ip_address = sf::IpAddress(sf::IpAddress::getLocalAddress());
static const uint16_t DEFAULT_PORT = 8080;

enum Directions
{
    Rest,
    Left,
    Right,
    Down,
    Up
};

enum ObjectTypes
{
    Empty,
    Player
};

}
