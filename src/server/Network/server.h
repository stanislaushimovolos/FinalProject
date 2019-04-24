#pragma once

#include "../Manager/BaseManager.h"
#include "client_handler.h"
#include "../../configuration/config.h"

#include <list>
#include <iostream>
#include <SFML/Network.hpp>
#include <iostream>
#include <unistd.h>

namespace ser
{

class Server
{

 public:
    explicit Server(uint16_t port = conf::net::DEFAULT_PORT,
                    uint32_t max_num_of_players = conf::net::REQUIRED_NUM_OF_CLIENTS,
                    uint32_t connection_delay = conf::net::CONNECTION_DELAY);

    int connect_clients();

    int start_session(BaseManager &manager);

    ~Server();

 private:
    uint16_t _port;
    sf::TcpListener _listener;
    sf::IpAddress _ip_address;
    sf::SocketSelector _selector;
    std::list<Handler> _clients;

    uint32_t _max_num_of_players;
    uint32_t _current_num_of_clients;
    uint32_t _connection_delay;

    std::vector<Packet> receive_packets();

    int add_new_client();

    int send_state_to_ready_sockets(sf::Packet &current_state);
};

}