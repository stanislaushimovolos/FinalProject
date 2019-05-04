#include <iostream>
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

#include "server/Network/Server.h"
#include "server/Managers/LevelManager.h"

#include "client/Network/Client.h"


int main()
{

    std::string type;
    std::cout << "Enter type of connection" << std::endl;
    std::cin >> type;

    if (type == "s")
    {
        ser::GameManager game_manager(conf::map::server_map_path);
        ser::Server server
            (conf::net::DEFAULT_PORT, conf::net::CONNECTION_DELAY, game_manager.count_players());

        server.connect_clients();
        server.start_session(game_manager);
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
}
