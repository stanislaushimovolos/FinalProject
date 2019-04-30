#include "Manager.h"

namespace ser
{

int Manager::add_players(const std::list<ser::Handler> &clients)
{
    _players_states.reserve(clients.size());
    for (auto &cli:clients)
    {
        auto id = ser::ClientId(cli.get_id());
        _players[id] = new Player(id.get_id());
        _objects.push_back(_players[id]);
    }
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


int Manager::update_player_states(std::vector<ser::Packet> &received_data)
{
    process_packets(received_data);
    for (auto player_state:_players_states)
    {
        auto &player = _players[player_state._id];
        auto current_player_direction = player_state.direction;
        player->set_speed_from_direction(current_player_direction);

        auto is_player_shoot = player_state.is_shoot;
        if (is_player_shoot)
        {
            if (player->_shoot_clicks < conf::game::one_shoot_required_clicks)
            {
                if (player->_shoot_clicks == 0)
                    _objects.push_back(new Bullet(player->get_position(), player->get_rotation()));
                player->_shoot_clicks++;
            } else
                player->_shoot_clicks = 0;
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


Manager::~Manager()
{
    for (auto obj:_objects)
        delete obj;
}

}
