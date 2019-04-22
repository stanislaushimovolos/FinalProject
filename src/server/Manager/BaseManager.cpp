#include "BaseManager.h"


std::vector<std::pair<sf::Uint16, ser::Info >> BaseManager::
process_packets(std::vector<ser::Packet> &received_data) const
{
    {
        std::vector<std::pair<sf::Uint16, ser::Info >> processed_messages;
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


int BaseManager::update_state(std::vector<ser::Packet> &received_data)
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


int BaseManager::add_players(const std::list<ser::Handler> &clients)
{
    {
        for (auto &cli:clients)
        {
            auto id = ser::Info(cli.info());
            players[id] = Player();
        }
        return 1;
    }
}


std::vector<ser::Packet> BaseManager::get_players_states()
{
    std::vector<ser::Packet> current_states;
    current_states.reserve(players.size());

    for (auto[key, value] : players)
    {
        ser::Packet packet(key.info());
        packet.data() << value.position.x << value.position.y;
        current_states.push_back(packet);
    }
    return current_states;
}