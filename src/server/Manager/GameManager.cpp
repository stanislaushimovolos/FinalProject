#include "GameManager.h"

namespace ser
{

int Manager::add_players(const std::list<ser::Handler> &clients)
{
    for (auto &cli:clients)
    {
        auto id = ser::ClientId(cli.get_id());
        _players[id] = new Player(id.get_id());
        _objects.push_back(_players[id]);
    }
    _objects.push_back(new MovingPlatform({200, 0},
                                          {conf::game::hole_speed, conf::game::hole_speed},
                                          300, conf::game::hole_damage));

    _objects.push_back(new MovingPlatform({0, 500},
                                          {conf::game::hole_speed, 0},
                                          300,
                                          conf::game::hole_damage));
    _objects.push_back(new MovingPlatform({-100, 100},
                                          {0, -conf::game::hole_speed},
                                          300,
                                          conf::game::hole_damage));
    return 1;
}


void Manager::process_packets(std::vector<ser::Packet> &received_data)
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


void Manager::collect_garbage()
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


int Manager::update_player_states(std::vector<ser::Packet> &received_data)
{
    process_packets(received_data);

    for (auto player_state:_players_states)
    {
        auto &player = _players[player_state._id];
        auto current_player_direction = player_state.direction;
        player->set_speed_from_direction(current_player_direction);

        if (player->is_live())
        {
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


int Manager::update_environment(sf::Time &&delta_t)
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
    return 1;
}


sf::Packet Manager::create_current_state_packet()
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


std::vector<uint64_t> Manager::get_players_ptr_id(const std::list<ser::Handler> &clients)
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


int Manager::remove_disconnected_players(std::vector<ClientId> &dis_players)
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


Manager::~Manager()
{
    for (auto obj:_objects)
        delete obj;
}

}
