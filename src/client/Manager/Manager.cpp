#include "Manager.h"

namespace cli
{

Manager::Manager(uint32_t x_resolution, uint32_t y_resolution, std::string &&window_name) :
    _resolution(x_resolution, y_resolution),
    _window_name(window_name),
    _is_window_opened(true),
    _is_window_focused(true),
    _view(sf::Vector2f(x_resolution / 2, y_resolution / 2),
          sf::Vector2f(x_resolution, y_resolution))
{
    _window.setView(_view);
    _objects.reserve(conf::game::START_NUM_OF_OBJECTS);
}


void Manager::set_remote_ip_port(std::pair<uint32_t, uint32_t> ip_port)
{
    this->_ip = ip_port.first;
    this->_port = ip_port.second;
}


sf::Packet Manager::get_current_state()
{
    sf::Event event;
    while (_window.pollEvent(event))
    {
        switch (event.type)
        {
            case sf::Event::Closed:
            {
                _is_window_opened = false;
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

    uint32_t current_direction = conf::game::Rest;
    uint32_t is_shoot = 0;

    if (_is_window_focused)
    {
        auto state = keyboard.get_direction();
        current_direction = state.first;
        is_shoot = state.second;
    }

    sf::Packet send_packet;
    send_packet << current_direction << is_shoot;
    return send_packet;
}


int Manager::process_scene(sf::Packet &packet)
{
    packet >> _current_num_of_objects;

    _objects.clear();
    if (_objects.size() < _current_num_of_objects)
        _objects.reserve(_current_num_of_objects);

    uint32_t obj_type = 0;
    for (int i = 0; i < _current_num_of_objects; i++)
    {
        packet >> obj_type;
        switch (obj_type)
        {
            case conf::game::Player:
            {
                uint32_t ip, port = 0;
                packet >> ip >> port;

                TextureDrawer sadad(obj_type, packet);
                _objects.emplace_back(obj_type, packet);

                if (ip == _ip && port == _port)
                    _view.setCenter(_objects.back().get_position());

                break;
            }
            case conf::game::Bullet:
            {
                _objects.emplace_back(obj_type, packet);
                break;
            }
            default:throw std::runtime_error("unknown type of object");
        }
    }
    return 0;
}


void Manager::update(sf::Packet &packet)
{
    process_scene(packet);
    _window.setView(_view);
    draw();
}


void Manager::draw()
{
    for (auto &obj:_objects)
        obj.draw(_window);

    _window.display();
    _window.clear();
}


bool Manager::is_active()
{
    return _is_window_opened;
}


void Manager::activate_window()
{
    _window.create(sf::VideoMode(_resolution.x, _resolution.y), _window_name);
}

}
