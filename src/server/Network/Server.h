#pragma once

#include <list>
#include <iostream>
#include <SFML/Network.hpp>

#include "ClientHandler.h"
#include "../Manager/Manager.h"
#include "../../configuration/network.h"

namespace ser
{

class Server
{

 public:
    explicit Server(uint16_t port = conf::net::DEFAULT_PORT,
                    uint32_t max_num_of_players = conf::net::REQUIRED_NUM_OF_CLIENTS,
                    uint32_t connection_delay = conf::net::CONNECTION_DELAY);

    int start_session(Manager &manager);

    int connect_clients();

    ~Server();

 private:
    uint16_t _port;
    sf::TcpListener _listener;
    sf::IpAddress _ip_address;
    sf::SocketSelector _selector;
    std::list<Handler> _clients;

    std::vector<ClientId> _disconnected_clients;

    uint32_t _required_num_of_clients;
    uint32_t _current_num_of_clients;
    uint32_t _connection_delay;

    std::vector<Packet> _received_data;

    void receive_packets();

    int add_new_client();

    int send_id_to_clients(std::vector<uint64_t> ids);

    int send_state_to_clients(sf::Packet &current_state);
};

}