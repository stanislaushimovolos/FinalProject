#pragma once
#include <iostream>

namespace config
{
static const uint16_t DEFAULT_PORT = 8081;
static const uint32_t REQUIRED_NUM_OF_PLAYERS = 1;
static const uint32_t CONNECTION_DELAY = 20;

enum Directions
{
    Left,
    Right,
    Down,
    Up
};
}