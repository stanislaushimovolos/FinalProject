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

    _graph_objects(conf::game::START_NUM_OF_OBJECTS)
{
    _window.setView(_view);
}


void Manager::set_id(uint64_t id)
{
    _id = id;
}


void Manager::load_textures()
{
    using namespace conf::render;

    // fill texture map
    _textures[DevilTexture].loadFromFile(devil_texture_relative_path);
    _textures[BulletTexture].loadFromFile(fire_ball_texture_relative_path);
    _textures[GirlTexture].loadFromFile(girl_texture_relative_path);
    _textures[BoyTexture].loadFromFile(boy_texture_relative_path);
    _textures[GhostTexture].loadFromFile(ghost_texture_relative_path);

    for (auto &obj: _graph_objects)
        obj.set_texture_map(&_textures);
}


int Manager::process_scene(sf::Packet &packet)
{
    packet >> _current_num_of_objects;

    // Check if it is necessary to allocate more memory
    if (_graph_objects.capacity() < _current_num_of_objects)
    {
        _graph_objects = std::vector<SpriteDrawer>(2 * _current_num_of_objects);
        for (auto &obj: _graph_objects)
            obj.set_texture_map(&_textures);
    }

    uint32_t obj_type = 0;
    uint32_t num_of_properties = 0;
    float cur_object_coord_x = 0, cur_object_coord_y = 0;

    for (int i = 0; i < _current_num_of_objects; i++)
    {
        packet >> obj_type;
        switch (obj_type)
        {
            case conf::game::Player:
            {
                uint64_t current_player_id = 0;
                packet >> current_player_id >> cur_object_coord_x >> cur_object_coord_y;

                if (current_player_id == _id)
                    _view.setCenter(cur_object_coord_x, cur_object_coord_y);
                break;
            }
            case conf::game::Bullet:
            {
                packet >> cur_object_coord_x >> cur_object_coord_y;
                break;
            }
            default:
            {
                std::cout << "received unknown object" << std::endl;
                return 0;
            }
        }

        // one object may have several graphical properties
        packet >> num_of_properties;

        for (int j = 0; j < num_of_properties; j++)
        {
            uint32_t property_type = 0;
            packet >> property_type;
            _graph_objects[i].set_state_form_packet(packet);
        }
    }
    return 1;
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
            case sf::Event::Resized:
            {
                _view.setSize(event.size.width, event.size.height);
            }
            default:break;
        }
    }

    uint32_t current_direction = conf::game::Rest;
    uint32_t is_shoot = 0;

    if (_is_window_focused)
    {
        auto state = _keyboard.get_direction();
        current_direction = state.first;
        is_shoot = state.second;
    }

    sf::Packet send_packet;
    send_packet << current_direction << is_shoot;
    return send_packet;
}


void Manager::draw()
{
    sf::Vector2f view_coord = _view.getCenter();
    for (int i = 0; i < _current_num_of_objects; i++)
    {
        // draw close objects
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


int Manager::update(sf::Packet &packet)
{
    auto status = process_scene(packet);
    if (!status)
        return 0;

    _window.setView(_view);
    this->draw();
    return 1;
}


void Manager::activate()
{
    _window.create(sf::VideoMode(_resolution.x, _resolution.y), _window_name);
    load_textures();
}


bool Manager::is_window_active()
{
    return _is_window_opened;
}


uint64_t make_long_long(uint32_t first_bits, uint32_t last_bits)
{
    return first_bits | ((uint64_t) last_bits << 8 * sizeof(int));
}


sf::Packet &operator>>(sf::Packet &packet, uint64_t &number)
{
    uint32_t first_part, second_part;
    packet >> first_part >> second_part;

    number = make_long_long(first_part, second_part);
    return packet;
}

}
