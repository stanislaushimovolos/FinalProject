#include "server.h"

namespace ser
{

Server::Server(uint16_t port, uint32_t max_num_of_players, uint32_t connection_delay) :
    _port(port),
    _ip_address(sf::IpAddress::getLocalAddress()),
    _current_num_of_clients(0),
    _connection_delay(connection_delay),
    _max_num_of_players(max_num_of_players)
{
    std::cout << "Server ip is: " << _ip_address << std::endl;
    _listener.listen(_port);
    _selector.add(_listener);
}


int Server::connect_clients()
{
    while (true)
    {
        // Make the selector wait for data on any socket
        if (_selector.wait())
        {
            // Test the listener
            if (_selector.isReady(_listener))
                add_new_client();
            if (_current_num_of_clients == _max_num_of_players)
                return 1;
        }
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

        _current_num_of_clients++;
        std::cout << "new client connected" << std::endl;
        std::cout << "current number of players - " << _current_num_of_clients << std::endl;
        return 1;
    }

    delete new_client_socket;
    return -1;
}


std::vector<Packet> Server::receive_packets()
{
    std::vector<Packet> received_data;
    received_data.reserve(_clients.size());

    int client_counter = 0;
    for (auto it = _clients.begin(); it != _clients.end(); ++it)
    {
        auto &client = *it;
        auto client_socket_ptr = client.get_socket_ptr();
        if (_selector.isReady(*client_socket_ptr))
        {
            client_counter++;
            // The client has sent some data, we can receive it
            Packet pkg(client.info());

            if (client_socket_ptr->receive(pkg.data()) == sf::Socket::Done)
                received_data.push_back(pkg);
            else
            {
                _selector.remove(*client_socket_ptr);
                it = _clients.erase(it);

                std::cout << "client was disconnected" << std::endl;
                _current_num_of_clients--;
            }
        }
    }
    return received_data;
}


int Server::start_session(BaseManager &manager)
{
    sf::Clock clock;
    sf::Packet current_state;
    manager.add_players(_clients);

    while (true)
    {
        auto time = clock.getElapsedTime().asMilliseconds();
        if (time > _connection_delay)
        {
            send_state_to_ready_sockets(current_state);
            if (_selector.wait())
            {
                auto received_packets = receive_packets();
                manager.update_state(received_packets);
                current_state = manager.create_current_state_packet();
            }
            clock.restart();
        }
    }
}


int Server::send_state_to_ready_sockets(sf::Packet &current_state)
{
    for (auto &it:_clients)
    {
        auto &client = it;
        auto client_socket_ptr = client.get_socket_ptr();
        client_socket_ptr->send(current_state);
    }
    return 1;
}


Server::~Server()
{
    std::cout << "server was destroyed!!" << std::endl;
}

}