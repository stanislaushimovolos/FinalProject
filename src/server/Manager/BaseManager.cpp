#include "BaseManager.h"

namespace ser
{

std::vector<std::pair<sf::Uint16, ser::Info >> BaseManager::
process_packets(std::vector<ser::Packet> &received_data) const
{
    std::vector<std::pair<sf::Uint16, ser::Info >> processed_messages;
    processed_messages.reserve(players.size());

    for (auto &msg: received_data)
    {
        uint32_t direction = 0;
        msg.data() >> direction;
        processed_messages.emplace_back(direction, msg.info());
    }

    return processed_messages;
}


int BaseManager::update_state(std::vector<ser::Packet> &received_data)
{
    auto players_states = process_packets(received_data);

    for (auto[dir, info]:players_states)
    {
        auto &player = players[info];
        switch (dir)
        {
            case (config::Left):
            {
                player.move(sf::Vector2f(-20, 0));
                break;
            }
            case (config::Right):
            {
                player.move(sf::Vector2f(20, 0));
                break;
            }
            case (config::Up):
            {
                player.move(sf::Vector2f(0, -20));
                break;
            }
            case (config::Down):
            {
                player.move(sf::Vector2f(0, 20));
                break;
            }
            default:break;
        }
    }
    return 1;
}


int BaseManager::add_players(const std::list<ser::Handler> &clients)
{
    {
        for (auto &cli:clients)
        {
            auto id = ser::Info(cli.info());
            players[id] = ser::Player();
        }
        return 1;
    }
}


sf::Packet BaseManager::create_current_state_packet()
{
    uint32_t num_of_players = players.size();
    sf::Packet packet;
    packet << num_of_players;

    for (auto[player_info, player] : players)
    {
        auto[ip, port] = player_info.get_info();
        auto pos = player.get_position();
        packet << ip << port << pos.x << pos.y;
    }
    return packet;
}

}