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
        server.handle();
    } else
    {
        std::string name, message;
        auto ip = sf::IpAddress(sf::IpAddress::getLocalAddress());
        sf::TcpSocket socket;

        std::cout << "Enter your name" << std::endl;
        std::cin >> name;
        name += " ";

        socket.connect(ip, default_port);

        while (true)
        {
            std::cin >> message;
            sf::Packet packet;

            packet << name + message;
            socket.send(packet);
        }
    }
    return 0;
}