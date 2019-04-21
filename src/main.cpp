#include <iostream>
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

#include "server/server.h"


int main()
{
    std::string type;
    std::cout << "Enter type of connection" << std::endl;
    std::cin >> type;

    if (type == "s")
    {
        Server server(DEFAULT_PORT);
        server.connect_clients();
        server.start_session();
    } else
    {
        sf::RenderWindow Window(sf::VideoMode(800, 600, 32), "Test");
        sf::RectangleShape player;
        player.setSize(sf::Vector2f(40, 40));
        player.setFillColor(sf::Color::Cyan);

        auto ip = sf::IpAddress(sf::IpAddress::getLocalAddress());
        sf::TcpSocket socket;

        float x, y = 0;
        sf::Uint16 direction = 320;
        bool focused = true;

        socket.connect(ip, DEFAULT_PORT);
        while (Window.isOpen())
        {
            sf::Event event;
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
            receive_packet >> x >> y;
            player.setPosition(x, y);

            direction = 320;

            if (focused)
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                {
                    direction = Right;
                } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                {
                    direction = Left;
                } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                {
                    direction = Up;
                } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                {
                    direction = Down;
                }
            } else
                direction = 320;

            send_packet << direction;

            auto res = socket.send(send_packet);

            Window.draw(player);
            Window.display();
            Window.clear();
        }
    }
    return 0;
}