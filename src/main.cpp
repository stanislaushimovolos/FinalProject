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
    } else
    {
        std::string name, message;
        auto ip = sf::IpAddress(sf::IpAddress::getLocalAddress());
        sf::TcpSocket socket;

        socket.connect(ip, default_port);

        while (true)
        {
            std::cin >> message;
            sf::Packet packet;

            packet << message;
            socket.send(packet);
        }
    }
    return 0;
}