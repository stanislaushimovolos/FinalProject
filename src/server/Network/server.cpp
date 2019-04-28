#include "server.h"

namespace ser
{

Server::Server(uint16_t port, uint32_t max_num_of_players, uint32_t connection_delay) :
    _port(port),
    _ip_address(sf::IpAddress::getLocalAddress()),
    _current_num_of_clients(0),
    _connection_delay(connection_delay),
    _required_num_of_clients(max_num_of_players)
{
    std::cout << "Server ip is: " << _ip_address << std::endl;
    _listener.listen(_port);
    _selector.add(_listener);

    _received_data.reserve(max_num_of_players);
}


int Server::connect_clients()
{
    while (true)
    {
        // Make the selector wait for new connections
        if (_selector.wait())
        {
            // Test the listener
            if (_selector.isReady(_listener))
                add_new_client();
            if (_current_num_of_clients == _required_num_of_clients)
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


void Server::receive_packets()
{
    _received_data.clear();

    // Waiting for all clients to send data
    int client_counter = 0;
    while (true)
    {
        for (auto &cli:_clients)
        {
            auto client_socket_ptr = cli.get_socket_ptr();
            if (_selector.isReady(*client_socket_ptr))
                client_counter++;
        }
        if (client_counter == _required_num_of_clients)
            break;
    }

    for (auto it = _clients.begin(); it != _clients.end(); ++it)
    {
        auto &client = *it;
        auto client_socket_ptr = client.get_socket_ptr();
        if (_selector.isReady(*client_socket_ptr))
        {
            // The client has sent some data, we can receive it
            Packet pkg(client.info());

            if (client_socket_ptr->receive(pkg.data()) == sf::Socket::Done)
                _received_data.push_back(pkg);
            else
            {
                _selector.remove(*client_socket_ptr);
                it = _clients.erase(it);

                std::cout << "client was disconnected" << std::endl;
                _current_num_of_clients--;
            }
        }
    }
}


int Server::start_session(Manager &manager)
{
    sf::Clock clock;
    sf::Packet current_state;
    manager.add_players(_clients);

    while (true)
    {
        auto time = clock.getElapsedTime().asMilliseconds();
        if (time > _connection_delay)
        {
            send_state_to_clients(current_state);
            if (_selector.wait())
            {
                receive_packets();
                manager.update_player_states(_received_data);
                manager.update_environment();
                current_state = manager.create_current_state_packet();
            }
            clock.restart();
        }
    }
}


int Server::send_state_to_clients(sf::Packet &current_state)
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