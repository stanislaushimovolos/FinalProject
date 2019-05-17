#include "server/Network/Server.h"
#include "client/Network/Client.h"

// Remove if you do not use X Window System
#include <X11/Xlib.h>


#include "client/Menu/Menu.h"


int main()
{
    // Remove if you do not use X Window System
    XInitThreads();

    Menu menu(conf::render::x_resolution, conf::render::y_resolution);
    sf::RenderWindow window
        (sf::VideoMode(conf::render::x_resolution, conf::render::y_resolution), "Postavte 10 pls");

    menu.set_lines({"Host", "Join", "Exit"});
    auto user_choice = menu.update(window);
    window.close();

    if (user_choice == 0)
    {
        ser::GameManager game_manager(conf::map::map_path);
        ser::Server server
            (conf::net::DEFAULT_PORT,
             conf::net::CONNECTION_DELAY,
             game_manager.count_players());

        server.connect_clients();
        server.start_session(game_manager);
    } else if (user_choice == 1)
    {
        std::string s;
        std::cout << "Enter server IP: " << std::endl;
        std::cin >> s;
        sf::IpAddress remote_ip(s);

        cli::Client client(remote_ip, conf::net::DEFAULT_PORT);
        if (!client.connect())
            return EXIT_FAILURE;

        cli::Manager client_manager
            (conf::render::x_resolution, conf::render::y_resolution, "Postavte 10 pls");

        client.start_session(client_manager);
    }
    return 0;
}
