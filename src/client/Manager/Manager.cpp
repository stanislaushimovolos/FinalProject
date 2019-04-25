#include "Manager.h"

namespace cli
{

Manager::Manager(uint32_t x_resolution, uint32_t y_resolution, std::string &&window_name) :
    _resolution(x_resolution, y_resolution),
    _window_name(window_name),
    _is_window_opened(true),
    _is_window_focused(true)
{}


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
    packet >> _current_num_of_clients;
    for (auto obj: _objects)
        delete obj;

    _objects.clear();
    _objects.reserve(_current_num_of_clients);

    uint32_t obj_type;

    for (int i = 0; i < _current_num_of_clients; i++)
    {
        packet >> obj_type;
        switch (obj_type)
        {
            case conf::game::Player:
            {
                _objects.push_back(new Player);
                break;
            }
            case conf::game::Bullet:
            {
                _objects.push_back(new Bullet);
                break;
            }
            default:throw std::runtime_error("unknown type of objects");
        }
        _objects.back()->set_state_form_packet(packet);
    }
    return 0;
}


void Manager::update(sf::Packet &packet)
{
    process_scene(packet);
    draw();
}


void Manager::activate_window()
{
    _window.create(sf::VideoMode(_resolution.x, _resolution.y), _window_name);
}


void Manager::draw()
{
    for (auto &obj:_objects)
        obj->draw(_window);

    _window.display();
    _window.clear();
}


bool Manager::is_active()
{
    return _is_window_opened;
}


Manager::~Manager()
{
    for (auto obj:_objects)
        delete obj;
}

}
