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
        if (_selector.wait(sf::seconds(5)))
        {
            // Test the listener
            if (_selector.isReady(_listener))
            {
                // The listener is ready: add new client
                auto new_client = new sf::TcpSocket;
                add_new_client(new_client);
            } else
            {
                auto data = receive_data();
                if (data.empty())
                {
                    std::cout << "No clients, so bb" << std::endl;
                    return 1;
                }
                for (auto &pkg: data)
                {
                    std::string msg;
                    pkg >> msg;
                    std::cout << msg << std::endl;
                }
            }
        } else
            return 1;
    }
}

int Server::add_new_client(sf::TcpSocket *client)
{
    if (_listener.accept(*client) == sf::Socket::Done)
    {
        // Add the new client to the clients list
        _clients.push_back(client);

        std::cout << "new client connected" << std::endl;
        // Add the new client to the selector
        _selector.add(*client);
        return 1;
    }

    // Error, we won't get a new connection, delete the socket
    delete client;
    return -1;
}

std::vector<sf::Packet> Server::receive_data()
{
    std::vector<sf::Packet> received_data(_clients.size());

    for (auto it = _clients.begin(); it != _clients.end(); ++it)
    {
        sf::TcpSocket &client = **it;
        if (_selector.isReady(client))
        {
            // The client has sent some data, we can receive it
            sf::Packet packet;
            if (client.receive(packet) == sf::Socket::Done)
                received_data.push_back(std::move(packet));
            else
            {
                std::cout << "client was disconnected" << std::endl;
                _selector.remove(client);

                delete *it;
                it = _clients.erase(it);
            }
        }
    }
    return received_data;
}

int Server::process_received_data(const std::vector<sf::Packet> &received_data)
{

}

Server::~Server()
{
    std::cout << "server was destroyed" << std::endl;
    for (auto client: _clients)
        delete client;
}