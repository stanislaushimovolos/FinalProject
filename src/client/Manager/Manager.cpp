#include "Manager.h"

namespace cli
{

Manager::Manager(uint32_t x_resolution, uint32_t y_resolution, std::string &&window_name) :
    _resolution(x_resolution, y_resolution),
    _window_name(window_name),
    _is_window_opened(true),
    _is_window_focused(true),
    _view(sf::Vector2f(x_resolution / 2, y_resolution / 2),
          sf::Vector2f(x_resolution, y_resolution)),

    _graph_objects(conf::game::START_NUM_OF_OBJECTS),
    _textures(50)
{
    _window.
        setView(_view);
}


void Manager::set_remote_ip_port(std::pair<uint32_t, uint32_t> ip_port)
{
    this->_ip = ip_port.first;
    this->_port = ip_port.second;
}


void Manager::load_textures()
{
    _textures[0].loadFromFile("../client/Textures/Dude.png");
    _textures[1].loadFromFile("../client/Textures/FireBall.png");
    for (auto &obj: _graph_objects)
        obj._textures = &_textures;
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
    if (_graph_objects.capacity() < _current_num_of_objects)
        _graph_objects = std::vector<SpriteDrawer>(2 * _current_num_of_objects);

    uint32_t obj_type = 0;
    uint32_t num_of_properties = 0;
    float coord_x = 0, coord_y = 0;

    for (int i = 0; i < _current_num_of_objects; i++)
    {
        packet >> obj_type;
        switch (obj_type)
        {
            case conf::game::Player:
            {
                uint32_t ip, port = 0;
                packet >> ip >> port >> coord_x >> coord_y;

                if (ip == _ip && port == _port)
                    _view.setCenter(coord_x, coord_y);
                break;
            }
            case conf::game::Bullet:
            {
                packet >> coord_x >> coord_y;
                break;
            }
            default:throw std::runtime_error("unknown type of object");
        }

        packet >> num_of_properties;
        for (int j = 0; j < num_of_properties; j++)
        {
            uint32_t property_type = 0;
            packet >> property_type;
            _graph_objects[i].set_state_form_packet(packet);
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
    sf::Vector2f view_coord = _view.getCenter();
    for (int i = 0; i < _current_num_of_objects; i++)
    {
        auto obj_pos = _graph_objects[i].get_position();
        if (abs(obj_pos.x - view_coord.x) < _resolution.x
            && abs(obj_pos.y - view_coord.y) < _resolution.y)
        {
            _graph_objects[i].draw(_window);
        }
    }

    _window.display();
    _window.clear(sf::Color::Yellow);
}


void Manager::activate()
{
    _window.create(sf::VideoMode(_resolution.x, _resolution.y), _window_name);
    load_textures();
}


bool Manager::is_active()
{
    return _is_window_opened;
}

}
