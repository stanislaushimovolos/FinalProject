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
    _local_port = _socket.getLocalPort();
}


int Client::connect()
{
    // Try connect to server
    auto status = _socket.connect(_remote_ip, (uint16_t) _remote_port);
    if (status != sf::Socket::Done)
    {
        std::cout << "can't connect server" << std::endl;
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

    /*
    * The event loop (more precisely, the pollEvent or waitEvent function)
    * must be called in the same thread that created the window.
    * SFML 2.3 docs
    */

    // Create window and load textures
    manager.activate();

    // Start new thread for keyboard input
    sf::TcpSocket *socket_ptr = &_socket;
    Manager *manager_ptr = &manager;

    std::future<int> async_input = std::async
        (std::launch::async,
         [socket_ptr, manager_ptr]
         {
             while (true)
             {
                 sf::sleep(sf::milliseconds(2* conf::net::CONNECTION_DELAY));

                 if (manager_ptr->is_window_active())
                 {
                     sf::Packet pack = manager_ptr->get_user_input();
                     auto status = socket_ptr->send(pack);
                     if (status != sf::Socket::Done)
                     {
                         std::cout << "Disconnected" << std::endl;
                         return 0;
                     }
                 } else
                 {
                     std::cout << "is not active now" << std::endl;
                     return 0;
                 }
             }
         });

    // Send and receive data one by one while game is active
    while (manager.is_window_active())
    {
        // receive data
        connection_status = receive_packet(received_packet);
        if (!connection_status)
            break;

        // Update local environment
        connection_status = manager.update(received_packet);
        if (!connection_status)
            break;
    }

    async_input.get();
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
}

}
