#include "Server.h"

namespace ser
{

Server::Server(uint16_t port, uint32_t connection_delay, uint32_t max_num_of_players) :
    _port(port),
    _ip_address(sf::IpAddress::getLocalAddress()),
    _current_num_of_clients(0),
    _connection_delay(connection_delay),
    _required_num_of_clients(max_num_of_players)
{
    _listener.listen(_port);
    _selector.add(_listener);
    _received_data.reserve(max_num_of_players);

    std::cout << "Server ip is: " << _ip_address << std::endl;
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

    // Make it non-blocking
    new_client_socket->setBlocking(false);

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

    if (_selector.wait(sf::Time::Zero))
    {
        for (auto it = _clients.begin(); it != _clients.end();)
        {
            ser::Handler &client = *it;
            auto client_socket_ptr = client.get_socket_ptr();

            // Test if client sent data
            if (_selector.isReady(*client_socket_ptr))
            {
                // The client has sent some data, we can receive it
                sf::Packet packet;
                ser::Packet new_client_state(client.get_id());
                auto receive_status = client_socket_ptr->receive(packet);

                // If there are several packets,
                // we need to read the first one and remove others from socket
                if (receive_status == sf::Socket::Done)
                {
                    new_client_state.data() = packet;
                    while (client_socket_ptr->receive(packet) == sf::Socket::Done)
                        packet.clear();

                    _received_data.emplace_back(new_client_state);
                    it++;
                } else if
                    (receive_status == sf::Socket::Disconnected ||
                        receive_status == sf::Socket::Error)
                {
                    // Remove not responding client
                    _selector.remove(*client_socket_ptr);
                    it = _clients.erase(it);

                    // Information for manager
                    _disconnected_clients.emplace_back(client.get_id());
                    _current_num_of_clients--;

                    std::cout << "couldn't receive data" << std::endl;
                    std::cout << "client was disconnected" << std::endl;
                }
            } else
            {
                it++;
            }
        }
    }
}


int Server::start_session(GameManager &manager)
{
    manager.start_game(_clients);

    auto players_ptr_id = manager.get_players_ptr_id(_clients);
    send_id_to_clients(players_ptr_id);

    sf::Packet current_game_state;

    sf::Clock connection_timer;
    while (true)
    {
        auto time_after_last_connection = connection_timer.getElapsedTime().asMilliseconds();
        if (time_after_last_connection > _connection_delay)
        {
            manager.update_game(connection_timer.restart());
            send_state_to_clients(current_game_state);

            receive_packets();
            if (!_disconnected_clients.empty())
            {
                manager.remove_disconnected_players(_disconnected_clients);
                _disconnected_clients.clear();
            }

            manager.update_player_states(_received_data);
            current_game_state = manager.create_current_state_packet();
        }
    }
}


int Server::send_id_to_clients(std::vector<uint64_t> ids)
{
    int id_counter = 0;

    // Send clients their unique id
    for (auto &it:_clients)
    {
        sf::Packet id_packet;

        auto &client = it;
        auto client_socket_ptr = client.get_socket_ptr();
        auto cur_player_id = ids[id_counter];

        id_packet << cur_player_id;
        if (client_socket_ptr->send(id_packet) != sf::Socket::Done)
        {
            std::cout << "couldn't send id" << std::endl;
            return 0;
        }
        id_counter++;
    }
    return 1;
}


int Server::send_state_to_clients(sf::Packet &current_state)
{
    for (auto it = std::begin(_clients); it != std::end(_clients);)
    {
        auto &client = it;
        auto client_socket_ptr = client->get_socket_ptr();

        /* If sf::Socket::Partial is returned, you must make sure to handle the partial send properly
         * When sending sf::Packets, the byte offset is saved within the sf::Packet itself.
         * In this case, you we make sure to keep attempting to send the exact same unmodified
         * sf::Packet object over and over until a status other than sf::Socket::Partial is returned.
         * SFML 2.3 docs
         */

        auto status = client_socket_ptr->send(current_state);
        while (status == sf::Socket::Partial)
            status = client_socket_ptr->send(current_state);

        it++;

    }
    return 1;
}


Server::~Server()
{
    std::cout << "server was destroyed!!" << std::endl;
}

}