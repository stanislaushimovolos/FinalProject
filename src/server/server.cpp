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
        if (_selector.wait(sf::seconds(51)))
        {
            // Test the listener
            if (_selector.isReady(_listener))
                add_new_client();
            else
            {
                auto data = receive_data();
                process_received_data(std::move(data));
            }
        } else
            return 1;
    }
}

int Server::add_new_client()
{
    ClientHandler client;
    if (_listener.accept(*client._socket) == sf::Socket::Done)
    {
        // Add the new client to the selector
        _selector.add(*client._socket);

        // Add the new client to the clients list
        _clients.emplace_back(std::move(client));

        std::cout << "new client connected" << std::endl;
        return 1;
    }

    // Error, we won't get a new connection, delete the socket
    return -1;
}

std::vector<ClientPacket> Server::receive_data()
{
    std::vector<ClientPacket> received_data;

    for (auto it = _clients.begin(); it != _clients.end(); ++it)
    {
        auto &client = *it;
        if (_selector.isReady(*client._socket))
        {
            // The client has sent some data, we can receive it
            ClientPacket pkg(client._info);

            if (client._socket->receive(pkg.packet) == sf::Socket::Done)
                received_data.emplace_back(pkg);
            else
            {
                std::cout << "client was disconnected" << std::endl;
                _selector.remove(*client._socket);
                it = _clients.erase(it);
            }
        }
    }
    return received_data;
}

int Server::process_received_data(std::vector<ClientPacket> &&received_data)
{
    std::vector<std::pair<std::string, uint32_t >> processed_messages;
    for (auto &msg: received_data)
    {
        std::string data;
        msg.packet >> data;
        processed_messages.emplace_back(std::make_pair(data, msg._info._port));
    }

    for (auto &msg: processed_messages)
    {
        std::cout << msg.second << "  " << msg.first << std::endl;
    }
    return 1;
}

Server::~Server()
{
    std::cout << "server was destroyed" << std::endl;
}