#pragma once
#include <iostream>
#include "../Network/client_handler.h"

class EqualClients
{
 public:
    bool operator()(ser::Info const &first, ser::Info const &second) const
    {
        return first.info() == second.info();
    }
};

class ClientHasher
{
 public:
    size_t operator()(ser::Info const &cli) const
    {
        return cli.info().first;
    }
};

class Player
{
 public:
    Player() :
        position(0, 0),
        velocity(0, 0),
        speed(20)
    {}

    sf::Vector2f position;
    sf::Vector2f velocity;
    float speed;
 private:
};
