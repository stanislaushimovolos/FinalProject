#pragma once

#include "client_handler.h"

#include <list>
#include <iostream>
#include <SFML/Network.hpp>
#include <iostream>

const uint16_t default_port = 8081;
const uint32_t REQUIRED_NUM_OF_PLAYERS = 3;

class Server
{
 public:
    explicit Server(uint16_t port = default_port,
                    uint32_t max_num_of_players = REQUIRED_NUM_OF_PLAYERS);

    int connect_clients();

    ~Server();

 private:
    uint16_t _port;
    sf::TcpListener _listener;
    sf::IpAddress _ip_address;
    sf::SocketSelector _selector;
    std::list<ClientHandler> _clients;

    uint32_t _max_num_of_players;
    uint32_t _current_num_of_players;

    int add_new_client();

    int read_ready_sockets();

    std::vector<ClientPacket> receive_packets();
};