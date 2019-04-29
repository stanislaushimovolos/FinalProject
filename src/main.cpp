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
        try
        {
            auto remote_ip_port = sf::IpAddress::getLocalAddress();
            cli::Manager server_manager(800, 600, "Synchronized!!!!");
            cli::Client client(remote_ip_port, conf::net::DEFAULT_PORT);
            client.start_session(server_manager);

        }
        catch (std::exception &exception)
        {
            std::cout << exception.what() << std::endl;
        }
    }
    return 0;
}
