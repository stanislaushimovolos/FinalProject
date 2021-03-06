#pragma once

#include <SFML/Network.hpp>
#include <iostream>
#include <thread>
#include <unistd.h>

#include "../Manager/ClientManager.h"
#include "../../configuration/network.h"

namespace cli
{


// Class which provides communication with server

class Client
{
 public:

    Client(const sf::IpAddress &remote_ip, uint32_t remote_port);

    std::pair<uint32_t, uint32_t> get_local_ip_port();

    int receive_packet(sf::Packet &packet);

    int start_session(Manager &manager);

    int connect();

    int receive_id();

    ~Client();


 private:
    sf::TcpSocket _socket;
    sf::IpAddress _remote_ip;
    uint32_t _remote_port;

    uint32_t _local_port;
    uint32_t _local_ip;

    // Unique ID of client
    uint64_t _id;
};

void read_input(sf::TcpSocket *socket_ptr, Manager *manager_ptr);

}
