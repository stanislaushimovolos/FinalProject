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
                _is_window_focused = true;
                break;
            }
            case sf::Event::LostFocus:
            {
                _is_window_focused = false;
                break;
            }
            default:break;
        }
    }

    uint32_t current_direction = 0;
    if (_is_window_focused)
        current_direction = keyboard.get_direction();
    else
        current_direction = 320;

    process_packet(packet);

    sf::Packet send_packet;
    send_packet << current_direction;
    return send_packet;
}


int Manager::process_packet(sf::Packet &packet)
{
    packet >> _current_num_of_clients;
    _objects.clear();
    _objects.reserve(_current_num_of_clients);

    uint32_t current_ip_, current_port_ = 0;
    sf::Vector2f current_obj_position;

    for (int i = 0; i < _current_num_of_clients; i++)
    {
        packet >> current_ip_ >> current_port_;
        packet >> current_obj_position.x >> current_obj_position.y;
        _objects.emplace_back(current_obj_position);
    }

    draw();
    return 0;
}


void Manager::draw()
{
    for (auto &obj:_objects)
        obj.draw(_window);

    _window.display();
    _window.clear();
}

}
