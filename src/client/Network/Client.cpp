#include <iostream>
#include "Client.h"

namespace cli
{

Client::Client(const sf::IpAddress &remote_ip, uint32_t remote_port) :
    _remote_port(remote_port),
    _remote_ip(remote_ip)
{
    _local_ip = sf::IpAddress(sf::IpAddress::getLocalAddress()).toInteger();

    // Try connect to server
    auto status = _socket.connect(_remote_ip, (uint16_t) _remote_port);
    if (status != sf::Socket::Done)
        throw std::runtime_error("can't connect server");

    _local_port = _socket.getLocalPort();
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
    // Initialize connection
    manager.set_remote_ip_port(get_local_ip_port());
    auto received_pack = receive_packet();
    auto send_pack = manager.get_current_state();

    send_packet(send_pack);
    manager.activate();

    while (manager.is_active())
    {
        received_pack = receive_packet();
        manager.update(received_pack);

        send_pack = manager.get_current_state();
        send_packet(send_pack);
    }
    return 1;
}


std::pair<uint32_t, uint32_t> Client::get_local_ip_port()
{
    return std::make_pair(_local_ip, _local_port);
}

}
