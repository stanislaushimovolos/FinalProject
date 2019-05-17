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
    _top_border(0),
    _left_border(0),
    _right_border(0),
    _bottom_border(0),

    _graph_objects(conf::game::start_num_of_objects),
    _view(sf::Vector2f(x_resolution / 2, y_resolution / 2),
          sf::Vector2f(x_resolution, y_resolution)
    )
{
    _window.setView(_view);
    //menu
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

    //TODO : do better
    for (auto &tile_layer: _map_tile_layers)
    {
        _tiles.emplace_back(_level.GetMapSize().y, _level.GetMapSize().x);
        for (sf::Sprite &tile : tile_layer.tiles)
        {
            int column = (int) (tile.getPosition().x / _tile_size.x);
            int row = (int) (tile.getPosition().y / _tile_size.y);
            _tiles.back().sprites[row][column] = &tile;
        }

    }
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
    draw_map();
    draw_objects();

    _window.display();
    _window.clear();
}


void Manager::draw_objects()
{
    for (int i = 0; i < _number_of_graph_objects; i++)
    {
        // Draw only close objects
        auto obj_pos = _graph_objects[i].get_position();
        if (obj_pos.x >= _left_border - 2 * _tile_size.x
            && obj_pos.x <= _right_border + 2 * _tile_size.x
            && obj_pos.y >= _top_border - 5 * _tile_size.y
            && obj_pos.y <= _bottom_border + 2 * _tile_size.y)
        {
            _graph_objects[i].draw(_window);
        }
    }
}


void Manager::draw_map()
{
    int right_tile_idx = (int) (_right_border / _tile_size.x);
    int left_tile_idx = (int) (_left_border / _tile_size.x);
    if (right_tile_idx != _level.GetMapSize().x)
        right_tile_idx++;

    int top_tile_idx = (int) (_top_border / _tile_size.y);
    int bot_tile_idx = (int) (_bottom_border / _tile_size.y);
    if (bot_tile_idx != _level.GetMapSize().x)
        bot_tile_idx++;

    for (auto &layer:_tiles)
    {
        for (int i = top_tile_idx; i < bot_tile_idx; i++)
        {
            for (int j = left_tile_idx; j < right_tile_idx; j++)
                _window.draw(*layer.sprites[i][j]);
        }
    }
}


int Manager::update(sf::Packet &packet)
{
    auto status = process_scene(packet);
    if (!status)
        return 0;

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
                break;
            }
            default:break;
        }
    }

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

                    // Compute borders of visible area
                    _left_border = (view_coord.x - _resolution.x / 2);
                    _right_border = (view_coord.x + _resolution.x / 2);

                    _top_border = (view_coord.y - _resolution.y / 2);
                    _bottom_border = (view_coord.y + _resolution.y / 2);
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


Manager::~Manager()
{
    std::cout << "manager was destroyed!!" << std::endl;
}

}
