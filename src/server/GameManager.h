#pragma once

#include "client_handler.h"
#include "server.h"
#include <unordered_map>
#include <list>

enum Directions
{
    Up1,
    Down1,
    Left1,
    Right1
};

class EqualClients
{
 public:
    bool operator()(ClientInfo const &first, ClientInfo const &second) const
    {
        return first.info() == second.info();
    }
};

class ClientHasher
{
 public:
    size_t operator()(ClientInfo const &cli) const
    {
        return cli.info().first;
    }
};

class Player
{
 public:
    Player() :
        position(0, 0),
        velocity(0, 0),
        speed(10)
    {}

    sf::Vector2f position;
    sf::Vector2f velocity;
    float speed;
 private:
};

class GameManager
{
 private:
    std::unordered_map<ClientInfo, Player, ClientHasher, EqualClients>
        players;
 public:
    std::vector<std::pair<sf::Uint16,
                          ClientInfo >> process_packets(std::vector<ClientPacket> &received_data)
    const
    {
        std::vector<std::pair<sf::Uint16, ClientInfo >> processed_messages;
        processed_messages.reserve(players.size());

        for (auto &msg: received_data)
        {
            sf::Uint16 direction = 0;
            msg.data() >> direction;
            processed_messages.emplace_back(direction, msg.info());
        }
        return processed_messages;
    }

    int update_state(std::vector<ClientPacket> &received_data)
    {
        auto players_states = process_packets(received_data);
        for (auto &new_state : players_states)
        {
            auto[direction, info] = new_state;
            auto &player = players[info];

            switch (direction)
            {
                case (Left1):
                {
                    player.position += sf::Vector2f(-player.speed, 0);
                    break;
                }
                case (Right1):
                {
                    player.position += sf::Vector2f(player.speed, 0);
                    break;
                }
                case (Up1):
                {
                    player.position += sf::Vector2f(0, -player.speed);
                    break;
                }
                case (Down1):
                {
                    player.position += sf::Vector2f(0, player.speed);
                    break;
                }
                default:return 1;
            }
        }
        return 1;
    }

    std::vector<ClientPacket> get_players_states()
    {
        std::vector<ClientPacket> current_states;
        current_states.reserve(players.size());

        for (auto[key, value] : players)
        {
            ClientPacket packet(key.info());
            std::cout << value.position.x << " " << value.position.y << std::endl;
            packet.data() << value.position.x << value.position.y;
            current_states.push_back(packet);
        }
        return current_states;
    }

    int add_players(const std::list<ClientHandler> &clients)
    {
        std::cout << "add player" << std::endl;
        for (auto &cli:clients)
        {
            auto id = ClientInfo(cli.info());
            players[id] = Player();
        }
        return 1;
    }
};
