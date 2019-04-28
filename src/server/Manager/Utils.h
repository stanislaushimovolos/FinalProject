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
        return first.get_id() == second.get_id();
    }
};

class ClientHasher
{
 public:
    size_t operator()(ser::ClientId const &cli) const
    {
        return cli.get_id().second;
    }
};

class ClientState
{
 public:
    uint32_t is_shoot;
    uint32_t direction;

    ClientId _id;

    ClientState(uint32_t dir, uint32_t is_shoot, std::pair<uint32_t, uint32_t> id) :
        direction(dir),
        is_shoot(is_shoot),
        _id(id)
    {}
};

}
