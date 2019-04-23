#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include "../Manager/Manager.h"

namespace cli
{

class Client
{
 public:
    Client(const sf::IpAddress &remote_ip, uint16_t remote_port);

    int send_packet(sf::Packet &pack);

    int start_session(Manager &manager);

    sf::Packet receive_packet();


 private:

    sf::TcpSocket _socket;
    sf::IpAddress _remote_ip;
    uint16_t _remote_port;

    uint16_t _local_port;
    uint32_t _local_ip;
};

}
