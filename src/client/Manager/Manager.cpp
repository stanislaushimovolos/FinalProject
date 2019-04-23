#include "Manager.h"

namespace cli
{
Manager::Manager(uint32_t x_resolution, uint32_t y_resolution, std::string &&window_name) :
    _resolution(x_resolution, y_resolution),
    _window_name(window_name)
{
    _window.create(sf::VideoMode(_resolution.x, _resolution.y), _window_name);
}


sf::Packet Manager::make_step(sf::Packet &packet)
{
    sf::Event event;

    while (_window.pollEvent(event))
    {
        switch (event.type)
        {
            case sf::Event::Closed:
            {
                _window.close();
                break;
            }
            case sf::Event::GainedFocus:
            {
                _focused = true;
                break;
            }
            case sf::Event::LostFocus:
            {
                _focused = false;
                break;
            }
            default:break;
        }
    }
    uint32_t cur_dir = 0;
    std::cout << _focused << std::endl;
    if (_focused)
        cur_dir = keyboard.get_direction();
    else
        cur_dir = 320;

    process_packet(packet);

    sf::Packet send_packet;
    send_packet << cur_dir;


    return send_packet;

}


int Manager::process_packet(sf::Packet &packet)
{
    packet >> _current_num_of_clients;
    std::vector<sf::RectangleShape> players(_current_num_of_clients);
    for (auto &X:players)
    {
        X.setSize(sf::Vector2f(40, 40));
        X.setFillColor(sf::Color::Red);
    }

    std::vector<sf::Vector2f> positions(config::REQUIRED_NUM_OF_CLIENTS);

    uint32_t ip_, port_ = 0;
    for (int i = 0; i < _current_num_of_clients; i++)
    {
        packet >> ip_ >> port_;
        packet >> positions[i].x >> positions[i].y;
        players[i].setPosition(positions[i]);
    }

    for (auto &X:players)
        _window.draw(X);

    _window.display();
    _window.clear();

    return 0;
}

}
