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
        ser::BaseManager server_manager;
        ser::Server server(config::DEFAULT_PORT);
        server.connect_clients();
        server.start_session(server_manager);
    } else
    {
        cli::Manager server_manager(800, 600, "Synchronized!!!!");
        cli::Client client(config::remote_ip_address, config::DEFAULT_PORT);
        client.start_session(server_manager);
    }
    return 0;
}
