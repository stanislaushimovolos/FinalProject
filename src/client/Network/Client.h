#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include "../Manager/Manager.h"

namespace cli
{

class Client
{
 public:
    Client(const sf::IpAddress &remote_ip, uint32_t remote_port);

    std::pair<uint32_t, uint32_t> get_local_ip_port();

    int receive_packet(sf::Packet &packet);

    int send_packet(sf::Packet &pack);

    int start_session(Manager &manager);

    uint64_t receive_id();


 private:
    sf::TcpSocket _socket;
    sf::IpAddress _remote_ip;
    uint32_t _remote_port;

    uint32_t _local_port;
    uint32_t _local_ip;
};

}
