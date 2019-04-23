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
        ser::Server server(config::DEFAULT_PORT);
        server.connect_clients();
        server.start_session();
    } else
    {
        cli::Client client(config::remote_ip_address, config::DEFAULT_PORT);
        client.start_session();
    }
    return 0;
}
