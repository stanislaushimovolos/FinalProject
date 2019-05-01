#include <iostream>
#include "Client.h"

namespace cli
{

Client::Client(const sf::IpAddress &remote_ip, uint32_t remote_port) :
    _remote_port(remote_port),
    _remote_ip(remote_ip),
    _id(0)
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
        std::cout << "server doesn't response" << std::endl;
        return 0;
    }
    return 1;
}


int Client::receive_packet(sf::Packet &packet)
{
    packet.clear();
    auto status = _socket.receive(packet);
    if (status != sf::Socket::Done)
    {
        std::cout << "server doesn't response" << std::endl;
        return 0;
    }
    return 1;
}


int Client::start_session(Manager &manager)
{
    sf::Packet received_packet;

    // Initialize connection
    auto connection_status = receive_id();
    if (!connection_status)
        return 0;

    // assign client id
    manager.set_id(_id);

    connection_status = receive_packet(received_packet);
    if (!connection_status)
        return 0;

    auto send_pack = manager.get_current_state();

    connection_status = send_packet(send_pack);
    if (!connection_status)
        return 0;

    manager.activate();

    while (manager.is_window_active())
    {
        // receive data
        connection_status = receive_packet(received_packet);
        if (!connection_status)
            return 0;

        // update local environment
        connection_status = manager.update(received_packet);
        if (!connection_status)
            return 0;

        // send user input
        send_pack = manager.get_current_state();
        connection_status = send_packet(send_pack);
        if (!connection_status)
            return 0;
    }
    return 1;
}


int Client::receive_id()
{
    sf::Packet id_packet;
    auto status = _socket.receive(id_packet);

    if (status != sf::Socket::Done)
    {
        std::cout << "couldn't receive id" << std::endl;
        return 0;
    }

    id_packet >> _id;
    return 1;
}


std::pair<uint32_t, uint32_t> Client::get_local_ip_port()
{
    return std::make_pair(_local_ip, _local_port);
}


Client::~Client()
{
    std::cout << "client was destroyed!!!" << std::endl;
    _socket.disconnect();
}

}
