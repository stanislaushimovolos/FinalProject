#pragma once

#include "client_handler.h"
#include "GameManager.h"

#include <list>
#include <iostream>
#include <SFML/Network.hpp>
#include <iostream>
#include <unistd.h>

const uint16_t DEFAULT_PORT = 8081;
const uint32_t REQUIRED_NUM_OF_PLAYERS = 1;
const uint32_t CONNECTION_DELAY = 1;

enum Direction
{
    Up,
    Down,
    Left,
    Right
};

class Server
{

 public:
    explicit Server(uint16_t port = DEFAULT_PORT,
                    uint32_t max_num_of_players = REQUIRED_NUM_OF_PLAYERS,
                    uint32_t connection_delay = CONNECTION_DELAY);

    int connect_clients();

    int start_session();

    ~Server();

 private:
    uint16_t _port;
    sf::TcpListener _listener;
    sf::IpAddress _ip_address;
    sf::SocketSelector _selector;
    std::list<ClientHandler> _clients;

    uint32_t _max_num_of_players;
    uint32_t _current_num_of_players;
    uint32_t _connection_delay;

    GameManager manager;

    int add_new_client();

    int send_packets_to_ready_sockets(std::vector<ClientPacket> &received_data);

    std::vector<ClientPacket> receive_packets();
};