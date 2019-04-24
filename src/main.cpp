#include <iostream>
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

#include "server/Network/server.h"
#include "client/Network/Client.h"


int main()
{
    std::string type;
    std::cout << "Enter type of connection" << std::endl;
    std::cin >> type;

    if (type == "s")
    {
        ser::Manager server_manager;
        ser::Server server(conf::net::DEFAULT_PORT);
        server.connect_clients();
        server.start_session(server_manager);
    } else
    {
        auto remote_ip = sf::IpAddress::getLocalAddress();
        cli::Manager server_manager(1920, 1080, "Synchronized!!!!");
        cli::Client client(remote_ip, conf::net::DEFAULT_PORT);
        client.start_session(server_manager);
    }
    return 0;
}
