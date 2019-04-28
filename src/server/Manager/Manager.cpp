#include "Manager.h"

namespace ser
{

int Manager::add_players(const std::list<ser::Handler> &clients)
{
    _players_states.reserve(clients.size());
    for (auto &cli:clients)
    {
        auto id = ser::ClientId(cli.info());
        _players[id] = new ser::Player(id.get_info());
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

        msg.data() >> direction >> is_shooting;
        ClientState cli_state(direction, is_shooting);
        _players_states.emplace_back(cli_state, msg.info());
    }
}


int Manager::update_player_states(std::vector<ser::Packet> &received_data)
{
    process_packets(received_data);
    for (auto[cur_state, info]:_players_states)
    {
        auto &player = _players[info];
        auto cur_dir = cur_state.direction;
        auto is_shoot = cur_state.is_shoot;
        player->set_direction(cur_dir);

        if (is_shoot)
            _objects.push_back(new Bullet(player->get_position(), player->get_rotation()));
    }
    return 1;
}


int Manager::update_environment()
{
    for (auto &obj:_objects)
        obj->update();
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
