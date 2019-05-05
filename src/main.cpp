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
            // Remove if you do not use X Window System
            XInitThreads();

            std::string s;
            std::cout << "Enter server IP: " << std::endl;
            std::cin >> s;
            sf::IpAddress remote_ip(s);

            cli::Client client(remote_ip, conf::net::DEFAULT_PORT);
            cli::Manager server_manager(800, 600, "Synchronized!!!!");
            client.start_session(server_manager);
        }
        catch (std::exception &exception)
        {
            std::cout << exception.what() << std::endl;
        }
    }
}
