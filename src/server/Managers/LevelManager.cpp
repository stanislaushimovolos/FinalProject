#include "LevelManager.h"

namespace ser
{

std::vector<Object> &LevelManager::get_all_objects()
{
    _objects = _level.GetAllObjects();
    return _objects;
}


std::vector<Object> LevelManager::get_objects_by_name(std::string &&name)
{
    return _level.GetObjects(name);
}


size_t LevelManager::count_players()
{
    return _level.GetObjects(conf::map::player_object_name).size();
}


bool LevelManager::load_level(const std::string &file_name)
{
    auto res = _level.LoadFromFile(file_name);
    return res;
}

}
