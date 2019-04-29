#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

#include "../../configuration/config.h"

namespace cli
{

class KeyboardInterface
{
 private:
    uint32_t _direction;

 public:
    std::pair<uint32_t, uint32_t> get_direction() const;

    explicit KeyboardInterface();
};

}