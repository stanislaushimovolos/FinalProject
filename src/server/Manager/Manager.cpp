#include "Manager.h"


std::vector<std::pair<sf::Uint16, Info >> Manager::
process_packets(std::vector<Packet> &received_data) const
{
    {
        std::vector<std::pair<sf::Uint16, Info >> processed_messages;
        processed_messages.reserve(players.size());

        for (auto &msg: received_data)
        {
            sf::Uint16 direction = 0;
            msg.data() >> direction;
            processed_messages.emplace_back(direction, msg.info());
        }
        return processed_messages;
    }
}


int Manager::update_state(std::vector<Packet> &received_data)
{
    auto players_states = process_packets(received_data);
    for (auto &new_state : players_states)
    {
        auto[direction, info] = new_state;
        auto &player = players[info];

        switch (direction)
        {
            case (config::Left):
            {
                player.position += sf::Vector2f(-player.speed, 0);
                break;
            }
            case (config::Right):
            {
                player.position += sf::Vector2f(player.speed, 0);
                break;
            }
            case (config::Up):
            {
                player.position += sf::Vector2f(0, -player.speed);
                break;
            }
            case (config::Down):
            {
                player.position += sf::Vector2f(0, player.speed);
                break;
            }
            default:return 1;
        }
    }
    return 1;
}


int Manager::add_players(const std::list<Handler> &clients)
{
    {
        for (auto &cli:clients)
        {
            auto id = Info(cli.info());
            players[id] = Player();
        }
        return 1;
    }
}


std::vector<Packet> Manager::get_players_states()
{
    std::vector<Packet> current_states;
    current_states.reserve(players.size());

    for (auto[key, value] : players)
    {
        Packet packet(key.info());
        packet.data() << value.position.x << value.position.y;
        current_states.push_back(packet);
    }
    return current_states;
}