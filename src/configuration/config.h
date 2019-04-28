#pragma once
#include <iostream>
#include <SFML/Network.hpp>

namespace conf
{

namespace net
{

static const uint32_t REQUIRED_NUM_OF_CLIENTS = 2;
static const uint32_t CONNECTION_DELAY = 5;
static const uint16_t DEFAULT_PORT = 8080;
}

namespace game
{

static const uint64_t START_NUM_OF_OBJECTS = 1000000;

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
    Player,
    Bullet,
};

}

}
