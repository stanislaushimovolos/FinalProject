#include <iostream>
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

#include "server/Network/server.h"


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
        sf::RenderWindow Window(sf::VideoMode(800, 600, 32), "Test");

        auto ip = sf::IpAddress(sf::IpAddress::getLocalAddress());
        sf::TcpSocket socket;

        float x, y = 0;
        uint32_t direction = 320;
        bool focused = true;

        uint32_t ip_num = ip.toInteger();
        uint32_t port = socket.getLocalPort();

        std::vector<sf::RectangleShape> players(config::REQUIRED_NUM_OF_PLAYERS);
        for (auto &X:players)
        {
            X.setSize(sf::Vector2f(40, 40));
            X.setFillColor(sf::Color::Cyan);
        }
        std::cout << "init worked port - " << port << std::endl;
        socket.connect(ip, config::DEFAULT_PORT);
        sf::Event event;

        while (Window.isOpen())
        {
            while (Window.pollEvent(event))
            {
                switch (event.type)
                {
                    case sf::Event::Closed:
                    {
                        Window.close();
                        return 1;
                    }
                    case sf::Event::GainedFocus:
                    {
                        focused = true;
                        break;
                    }
                    case sf::Event::LostFocus:
                    {
                        focused = false;
                        break;
                    }
                    default:break;
                }
            }
            sf::Packet send_packet, receive_packet;
            socket.receive(receive_packet);

            uint32_t num_of_players = 0;
            uint32_t ip_, port_ = 0;

            receive_packet >> num_of_players;
            std::vector<sf::Vector2f> positions(config::REQUIRED_NUM_OF_PLAYERS);

            for (int i = 0; i < num_of_players; i++)
            {
                receive_packet >> ip_ >> port_;
                receive_packet >> positions[i].x >> positions[i].y;
                players[i].setPosition(positions[i]);
            }

            direction = 320;

            if (focused)
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                {
                    direction = config::Right;
                } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                {
                    direction = config::Left;
                } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                {
                    direction = config::Up;
                } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                {
                    direction = config::Down;
                }
            } else
                direction = 320;

            send_packet << direction;

            auto res = socket.send(send_packet);

            for (auto &X:players)
                Window.draw(X);

            Window.display();
            Window.clear();
        }
    }
    return 0;
}
