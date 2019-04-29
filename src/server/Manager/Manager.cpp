#include "Manager.h"

namespace ser
{

int Manager::add_players(const std::list<ser::Handler> &clients)
{
    _players_states.reserve(clients.size());
    for (auto &cli:clients)
    {
        auto id = ser::ClientId(cli.get_id());
        _players[id] = new ser::Player(id.get_id());
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
        auto is_player_shoot = player_state.is_shoot;

        player->set_speed_from_direction(current_player_direction);
        if (is_player_shoot)
            _objects.push_back(new Bullet(player->get_position(), player->get_rotation()));
    }
    return 1;
}


int Manager::update_environment(sf::Time &&delta_t)
{
    for (auto &obj:_objects)
        obj->update(delta_t.asMilliseconds());
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
