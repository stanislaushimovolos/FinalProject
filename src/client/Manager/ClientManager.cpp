#include "ClientManager.h"

namespace cli
{

Manager::Manager(uint32_t x_resolution, uint32_t y_resolution, std::string &&window_name) :
    _resolution(x_resolution, y_resolution),
    _window_name(window_name),
    _is_window_opened(true),
    _is_window_focused(true),
    _number_of_graph_objects(0),
    _tile_size(0, 0),
    _map_width(0),
    _map_height(0),

    _graph_objects(conf::game::start_num_of_objects),
    _view(sf::Vector2f(x_resolution / 2, y_resolution / 2),
          sf::Vector2f(x_resolution, y_resolution)
    )
{
    _window.setView(_view);
}


void Manager::activate()
{
    _window.create(sf::VideoMode(_resolution.x, _resolution.y), _window_name);
    load_textures_of_objects();

    // Check
    _level.LoadFromFile(conf::map::map_path);
    _tile_size = _level.GetTileSize();

    _map_tile_layers = _level.GetLayers();
    _map_width = _level.GetMapSize().x * _tile_size.x;
    _map_height = _level.GetMapSize().y * _tile_size.y;
}


void Manager::load_textures_of_objects()
{
    using namespace conf::render;

    // TODO : make it automatically. Add error checking.

    // fill texture map
    _textures[BoyTexture].loadFromFile(boy_texture_relative_path);
    _textures[GirlTexture].loadFromFile(girl_texture_relative_path);
    _textures[BlastTexture].loadFromFile(blast_texture_relative_path);
    _textures[GhostTexture].loadFromFile(ghost_texture_relative_path);
    _textures[DevilTexture].loadFromFile(devil_texture_relative_path);
    _textures[BulletTexture].loadFromFile(fire_ball_texture_relative_path);
    _textures[BlackHoleTexture].loadFromFile(black_hole_relative_path);

    // Give graph objects information about textures
    for (auto &obj: _graph_objects)
        obj.set_texture_map(&_textures);
}


void Manager::draw_scene()
{
    // Draw objects in view
    sf::Vector2f view_coord = _view.getCenter();
    draw_map(view_coord);
    draw_objects(view_coord);

    _window.display();
    _window.clear(sf::Color::Black);
}


void Manager::draw_objects(const sf::Vector2f &view_coord)
{
    for (int i = 0; i < _number_of_graph_objects; i++)
    {
        // Draw only close objects
        auto obj_pos = _graph_objects[i].get_position();
        if (abs(obj_pos.x - view_coord.x) < _resolution.x
            && abs(obj_pos.y - view_coord.y) < _resolution.y)
        {
            _graph_objects[i].draw(_window);
        }
    }
}


void Manager::draw_map(const sf::Vector2f &view_coord)
{
    // Compute borders of visible area
    float left_border = (view_coord.x - _resolution.x / 2 - _tile_size.x);
    float right_border = (view_coord.x + _resolution.x / 2 + _tile_size.x);

    float top_border = (view_coord.y - _resolution.y / 2 - _tile_size.y);
    float bottom_border = (view_coord.y + _resolution.y / 2 + _tile_size.y);

    for (auto &layer:_map_tile_layers)
    {
        for (auto &tile:layer.tiles)
        {
            auto tile_pos = tile.getPosition();
            if (tile_pos.x >= left_border && tile_pos.x <= right_border &&
                tile_pos.y >= top_border && tile_pos.y <= bottom_border)
            {
                _window.draw(tile);
            }
        }
    }
}


int Manager::update(sf::Packet &packet)
{
    auto status = process_scene(packet);
    if (!status)
        return 0;

    _window.setView(_view);
    this->draw_scene();
    return 1;
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

    _number_of_graph_objects = 0;
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
                {
                    sf::Vector2f view_coord(cur_object_coord_x, cur_object_coord_y);

                    // Don't allow view to go beyond border of map
                    if (view_coord.x + _resolution.x / 2 >= _map_width)
                        view_coord.x = _map_width - _resolution.x / 2;
                    else if (view_coord.x - _resolution.x / 2 < 0)
                        view_coord.x = _resolution.x / 2;

                    if (view_coord.y + _resolution.y / 2 >= _map_height)
                        view_coord.y = _map_height - _resolution.y / 2;
                    else if (view_coord.y - _resolution.y / 2 < 0)
                        view_coord.y = _resolution.y / 2;

                    _view.setCenter(view_coord);
                }

                break;
            }
            case conf::game::Bullet:
            {
                packet >> cur_object_coord_x >> cur_object_coord_y;
                break;
            }
            case conf::game::Blast:
            {
                packet >> cur_object_coord_x >> cur_object_coord_y;
                break;
            }
            case conf::game::MovingPlatform:
            {
                packet >> cur_object_coord_x >> cur_object_coord_y;
                break;
            }
            case conf::game::SolidBlock:
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

        // One object may have several graphical properties
        packet >> num_of_properties;

        for (int j = 0; j < num_of_properties; j++)
        {
            uint32_t property_type = 0;
            packet >> property_type;
            _graph_objects[_number_of_graph_objects++].set_state_form_packet(packet);
        }
    }
    return 1;
}


sf::Packet Manager::get_user_input()
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
                _resolution = _window.getSize();
            }
            default:break;
        }
    }

    // Check pressed keys
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


bool Manager::is_window_active()
{
    return _is_window_opened;
}


void Manager::set_id(uint64_t id)
{
    _id = id;
}


// Combine two 4 byte unsigned into 8 byte unsigned
uint64_t make_long_long(uint32_t first_bits, uint32_t last_bits)
{
    return first_bits | ((uint64_t) last_bits << 8 * sizeof(int));
}


// Overloaded operator to send 8 byte numbers
sf::Packet &operator>>(sf::Packet &packet, uint64_t &number)
{
    uint32_t first_part, second_part;
    packet >> first_part >> second_part;

    number = make_long_long(first_part, second_part);
    return packet;
}

}
