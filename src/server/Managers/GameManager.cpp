#include "GameManager.h"

namespace ser
{

GameManager::GameManager(const std::string &file_name)
{
    // Add check
    _level.load_level(file_name);
    _level.get_all_objects();
}


void GameManager::process_packets(std::vector<ser::Packet> &received_data)
{
    _players_states.clear();
    for (auto &msg: received_data)
    {
        uint32_t direction = 0;
        uint32_t is_shooting = 0;

        auto data_ref = msg.data();
        data_ref >> direction >> is_shooting;
        _players_states.emplace_back(direction, is_shooting, msg.get_id());
    }
}


void GameManager::collect_garbage()
{
    for (auto it = _objects.begin(); it != _objects.end();)
    {
        auto current_object = *it;
        if (!current_object->is_active())
        {
            if (current_object->get_type() == conf::game::Bullet)
            {
                auto bullet_owner = dynamic_cast<Bullet *>(current_object)->get_owner();
                _objects.push_back(new Blast(bullet_owner,
                                             current_object->get_position(),
                                             conf::game::blast_damage));
            }

            delete *it;
            it = _objects.erase(it);
        } else
            ++it;
    }
}


void GameManager::load_init_objects()
{
    // TODO : do it automatically
    auto black_hole_objects = _level.get_objects_by_name(conf::map::hole_object_name);
    for (auto &level_obj :black_hole_objects)
        _objects.push_back(new MovingPlatform(level_obj, conf::game::hole_speed));

    auto solid_blocks_objects = _level.get_objects_by_name(conf::map::solid_block_object_name);
    for (auto &obj :solid_blocks_objects)
        _objects.push_back(new SolidBlock(obj));

}


int GameManager::start_game(const std::list<ser::Handler> &clients)
{
    auto player_objects = _level.get_objects_by_name(conf::map::player_object_name);
    int player_counter = 0;

    for (auto &cli:clients)
    {
        auto id = ser::ClientId(cli.get_id());
        _players[id] = new Player(player_objects[player_counter]);
        _objects.push_back(_players[id]);
        player_counter++;
    }

    load_init_objects();
    return 1;
}


int GameManager::update_player_states(std::vector<ser::Packet> &received_data)
{
    process_packets(received_data);

    for (auto player_state:_players_states)
    {
        auto &player = _players[player_state._id];
        auto current_player_direction = player_state.direction;
        player->set_speed_from_direction(current_player_direction);

        if (player->is_live())
        {

            // Check if player is shooting
            uint32_t player_is_shoot = player_state.is_shoot;
            bool add_new_bullet = player->add_shoot_click(player_is_shoot);

            if (add_new_bullet)
                _objects.push_back(new Bullet(player->get_id(),
                                              player->get_position(),
                                              player->get_rotation()));
        }
    }
    return 1;
}


int GameManager::update_game(sf::Time &&delta_t)
{
    auto delta_t_milliseconds = delta_t.asMilliseconds();
    for (auto it = _objects.begin(); it != _objects.end(); ++it)
    {
        auto first_obj = (*it);
        for (auto jt = std::next(it); jt != _objects.end(); ++jt)
        {
            auto second_obj = (*jt);
            first_obj->interact(second_obj, delta_t_milliseconds);
        }
    }

    for (auto &obj:_objects)
        obj->update(delta_t_milliseconds);

    collect_garbage();

    if (_players.empty())
        return 0;
    return 1;
}


int GameManager::remove_disconnected_players(std::vector<ClientId> &dis_players)
{
    for (auto &id:dis_players)
    {
        for (auto it = _objects.begin(); it != _objects.end(); ++it)
        {
            if (*it == _players[id])
            {
                delete _players[id];
                _players.erase(id);
                _objects.erase(it);
                break;
            }
        }
    }
    return 0;
}


sf::Packet GameManager::create_current_state_packet()
{
    auto num_of_objects = (uint32_t) _objects.size();
    sf::Packet packet;
    packet << num_of_objects;

    for (auto obj : _objects)
    {
        packet << obj->get_type();
        obj->compress_to_packet(packet);
    }
    return packet;
}


std::vector<uint64_t> GameManager::get_players_ptr_id(const std::list<ser::Handler> &clients)
{
    std::vector<uint64_t> players_id;
    for (auto &cli:clients)
    {
        auto id = ser::ClientId(cli.get_id());
        auto ptr_id = _players[id]->get_id();
        players_id.push_back(ptr_id);
    }
    return players_id;
}


size_t GameManager::count_players()
{
    return _level.count_players();
}


GameManager::~GameManager()
{
    for (auto obj:_objects)
        delete obj;
}

}
