#include "Manager.h"

namespace ser
{

std::vector<std::pair<ClientState, Info >> Manager::
process_packets(std::vector<ser::Packet> &received_data) const
{
    std::vector<std::pair<ClientState, Info >> processed_messages;
    processed_messages.reserve(_players.size());

    for (auto &msg: received_data)
    {
        uint32_t direction = 0;
        uint32_t is_shooting = 0;

        msg.data() >> direction >> is_shooting;
        ClientState KKLudge(direction, is_shooting);
        processed_messages.emplace_back(KKLudge, msg.info());
    }
    return processed_messages;
}


int Manager::update_player_states(std::vector<ser::Packet> &received_data)
{
    auto players_states = process_packets(received_data);

    for (auto[cur_state, info]:players_states)
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
    uint32_t num_of_objects = _objects.size();
    sf::Packet packet;
    packet << num_of_objects;

    for (auto obj : _objects)
    {
        packet << obj->get_type();
        obj->to_packet(packet);
    }
    return packet;
}


Manager::~Manager()
{
    for (auto obj:_objects)
        delete obj;
}

}