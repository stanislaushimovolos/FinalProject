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


    // Start new thread for keyboard input
    /* Create window and load textures
    * The event loop (more precisely, the pollEvent or waitEvent function)
    * must be called in the same thread that created the window.
    * SFML 2.3 docs
    */
    manager.activate();
    sf::TcpSocket *socket_ptr = &_socket;
    std::future<int> async_input = std::async
        (std::launch::async,
         [socket_ptr, &manager]
         {
             while (true)
             {
                 sf::sleep(sf::milliseconds(conf::net::CONNECTION_DELAY/2));

                 sf::Packet pack = manager.get_user_input();
                 auto status = socket_ptr->send(pack);
                 if (status != sf::Socket::Done)
                 {
                     std::cout << "Disconnected" << std::endl;
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
            return 0;

        // update local environment
        connection_status = manager.update(received_packet);
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
