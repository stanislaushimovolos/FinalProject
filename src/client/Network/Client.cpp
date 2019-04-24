#include <iostream>
#include "Client.h"

namespace cli
{

Client::Client(const sf::IpAddress &remote_ip, uint16_t remote_port) :
    _remote_port(remote_port),
    _remote_ip(remote_ip)
{
    _local_ip = sf::IpAddress(sf::IpAddress::getLocalAddress()).toInteger();
    _local_port = _socket.getLocalPort();

    // Try connect to server
    auto status = _socket.connect(_remote_ip, _remote_port);
    if (status != sf::Socket::Done)
        throw std::runtime_error("can't connect server");
}


int Client::send_packet(sf::Packet &pack)
{
    auto status = _socket.send(pack);
    if (status != sf::Socket::Done)
    {
        std::cout << "couldn't send packet" << std::endl;
        return -1;
    }
    return 1;
}


sf::Packet Client::receive_packet()
{
    sf::Packet received_pack;
    auto status = _socket.receive(received_pack);
    if (status != sf::Socket::Done)
    {
        std::cout << "couldn't send packet" << std::endl;
        throw std::runtime_error("can't receive packet");
    }
    return received_pack;
}


int Client::start_session(Manager &manager)
{
    while (manager.is_active())
    {
        auto received_pack = receive_packet();
        manager.update(received_pack);

        auto send_pack = manager.get_current_state();
        send_packet(send_pack);
    }
    return 1;
}

}
