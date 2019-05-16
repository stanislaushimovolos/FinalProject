#include "server/Network/Server.h"
#include "client/Network/Client.h"

// Remove if you do not use X Window System
#include <X11/Xlib.h>


int main()
{

    std::string type;
    std::cout << "Enter type of connection" << std::endl;
    std::cin >> type;

    if (type == "s")
    {
        ser::GameManager game_manager(conf::map::map_path);
        ser::Server server
            (conf::net::DEFAULT_PORT,
             conf::net::CONNECTION_DELAY,
             game_manager.count_players());

        server.connect_clients();
        server.start_session(game_manager);
    } else
    {
        try
        {
            std::string s;
            std::cout << "Enter server IP: " << std::endl;
            std::cin >> s;
            sf::IpAddress remote_ip(s);

            cli::Client client(remote_ip, conf::net::DEFAULT_PORT);
            if (!client.connect())
                return EXIT_FAILURE;

            // Remove if you do not use X Window System
            XInitThreads();

            cli::Manager client_manager
                (conf::render::x_resolution, conf::render::y_resolution, "Postavte 10 pls");

            client.start_session(client_manager);
        }
        catch (std::exception &exception)
        {
            std::cout << exception.what() << std::endl;
        }
    }
}
