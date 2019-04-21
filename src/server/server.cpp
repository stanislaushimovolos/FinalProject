#include "server.h"


Server::Server(uint16_t port) :
    _port(port),
    _ip_address(sf::IpAddress::getLocalAddress())
{
    std::cout << "Server ip is: " << _ip_address << std::endl;
    _listener.listen(_port);
    _selector.add(_listener);
}


int Server::handle()
{
    while (true)
    {
        // Make the selector wait for data on any socket
        if (_selector.wait(sf::seconds(15)))
        {
            // Test the listener
            if (_selector.isReady(_listener))
                add_new_client();
            else
                read_ready_sockets();
        } else
            return 1;
    }
}


int Server::add_new_client()
{
    // Create new socket
    auto new_client_socket = new sf::TcpSocket;

    if (_listener.accept(*new_client_socket) == sf::Socket::Done)
    {
        // Add the new client to the selector
        _selector.add(*new_client_socket);
        // Add the new client to the clients list
        _clients.emplace_back(new_client_socket);

        std::cout << "new client connected" << std::endl;
        return 1;
    }
    // Error, we won't get a new connection, delete the socket
    return -1;
}


std::vector<ClientPacket> Server::receive_packets()
{
    std::vector<ClientPacket> received_data;

    for (auto it = _clients.begin(); it != _clients.end(); ++it)
    {
        auto &client = *it;
        auto client_socket_ptr = client.get_socket_ptr();

        if (_selector.isReady(*client_socket_ptr))
        {
            // The client has sent some data, we can receive it
            ClientPacket pkg(client.info());

            if (client_socket_ptr->receive(pkg.data()) == sf::Socket::Done)
                received_data.emplace_back(pkg);
            else
            {
                _selector.remove(*client_socket_ptr);
                it = _clients.erase(it);

                std::cout << "client was disconnected" << std::endl;
            }
        }
    }
    return received_data;
}


int Server::read_ready_sockets()
{
    auto received_data = receive_packets();
    std::vector<std::pair<std::string, ClientInfo >> processed_messages;

    for (auto &msg: received_data)
    {
        std::string data;
        msg.data() >> data;
        std::cout << msg.info().second;
        processed_messages.emplace_back(std::make_pair(data, msg.info()));
    }

    for (auto &msg: processed_messages)
        std::cout << "  " << msg.first << std::endl;
    return 1;
}


Server::~Server()
{
    std::cout << "server was destroyed!!" << std::endl;
}