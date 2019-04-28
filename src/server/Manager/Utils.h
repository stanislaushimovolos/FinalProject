#pragma once
#include <iostream>
#include "../Network/client_handler.h"

namespace ser
{

class EqualClients
{
 public:
    bool operator()(ser::ClientId const &first, ser::ClientId const &second) const
    {
        return first.get_info() == second.get_info();
    }
};

class ClientHasher
{
 public:
    size_t operator()(ser::ClientId const &cli) const
    {
        return cli.get_info().second;
    }
};

class ClientState
{
 public:
    uint32_t is_shoot;
    uint32_t direction;

    ClientState(uint32_t dir, uint32_t is_shoot) :
        direction(dir),
        is_shoot(is_shoot)
    {}
};

}
