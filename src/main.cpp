#include <iostream>
#include <SFML/Network.hpp>

#include "server/server.h"


int main()
{
    std::string type;
    std::cout << "Enter type of connection" << std::endl;
    std::cin >> type;

    if (type == "s")
    {
        Server server(default_port);
        server.connect_clients();
        server.start_session();
    } else
    {
        auto ip = sf::IpAddress(sf::IpAddress::getLocalAddress());
        sf::TcpSocket socket;
        std::string message = "ping", received;
        sf::Packet send_packet, receive_packet;
        send_packet << message;

        socket.connect(ip, default_port);
        while (true)
        {
            socket.receive(receive_packet);
            receive_packet >> received;
            auto res = socket.send(send_packet);
            std::cout << res << std::endl;
        }
    }
    return 0;
}