#pragma once

#include "client_handler.h"

#include <list>
#include <iostream>
#include <SFML/Network.hpp>
#include <iostream>

const uint16_t default_port = 8081;

class Server
{
 public:
    explicit Server(uint16_t port = default_port);

    int handle();

    ~Server();

 private:
    sf::TcpListener _listener;
    sf::IpAddress _ip_address;
    sf::SocketSelector _selector;
    std::list<ClientHandler> _clients;

    uint16_t _port;

    int add_new_client();

    int read_ready_sockets();

    std::vector<ClientPacket> receive_packets();
};