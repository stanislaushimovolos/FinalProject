#include "Manager.h"

namespace ser
{

std::vector<std::pair<sf::Uint16, ser::Info >> Manager::
process_packets(std::vector<ser::Packet> &received_data) const
{
    std::vector<std::pair<sf::Uint16, ser::Info >> processed_messages;
    processed_messages.reserve(_players.size());

    for (auto &msg: received_data)
    {
        uint32_t direction = 0;
        msg.data() >> direction;
        processed_messages.emplace_back(direction, msg.info());
    }

    return processed_messages;
}


int Manager::update_player_states(std::vector<ser::Packet> &received_data)
{
    auto players_states = process_packets(received_data);

    for (auto[cur_dir, info]:players_states)
    {
        auto &player = _players[info];
        player->set_direction(cur_dir);
    }
    return 1;
}


int Manager::add_players(const std::list<ser::Handler> &clients)
{
    {
        for (auto &cli:clients)
        {
            auto id = ser::Info(cli.info());
            _players[id] = new ser::Player;
            _objects.push_back(_players[id]);
        }
        return 1;
    }
}


int Manager::update_environment()
{
    for (auto &obj:_objects)
        obj->update();

    return 1;
}


sf::Packet Manager::create_current_state_packet()
{
    uint32_t num_of_players = _players.size();
    sf::Packet packet;
    packet << num_of_players;

    for (auto obj : _objects)
        obj->to_packet(packet);
    return packet;
}


Manager::~Manager()
{
    for (auto obj:_objects)
        delete obj;
}

}