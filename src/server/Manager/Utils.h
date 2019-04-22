#pragma once
#include <iostream>
#include "../Network/client_handler.h"

class EqualClients
{
 public:
    bool operator()(ser::Info const &first, ser::Info const &second) const
    {
        return first.get_info() == second.get_info();
    }
};

class ClientHasher
{
 public:
    size_t operator()(ser::Info const &cli) const
    {
        return cli.get_info().second;
    }
};
