#pragma once

#include "../../map/Level.h"
#include "../../configuration/map.h"

namespace ser
{

class LevelManager
{
 public:

    bool load_level(const std::string &file_name);

    std::vector<Object> get_objects_by_name(std::string &&name);

    std::vector<Object> &get_all_objects();

    size_t count_players();

 private:

    Level _level;

    std::vector<Object> _objects;

};

}

