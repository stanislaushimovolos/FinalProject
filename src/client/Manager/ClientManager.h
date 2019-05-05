#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_set>
#include <future>

#include "KeyboardInterface.h"
#include "../Render/SpriteDrawer.h"
#include "../../configuration/render.h"
#include "../../configuration/map.h"
#include "../../map/Level.h"

namespace cli
{

class Manager
{

 public:

    Manager(uint32_t x_resolution, uint32_t y_resolution, std::string &&window_name);

    void draw_objects(const sf::Vector2f &view_coord);

    void draw_map(const sf::Vector2f &view_coord);

    void load_textures_of_objects();

    void set_id(uint64_t id);

    void draw_scene();

    void activate();

    int process_scene(sf::Packet &packet);

    int update(sf::Packet &packet);

    sf::Packet get_user_input();

    bool is_window_active();

    ~Manager() = default;

    sf::View _view;

 private:

    // Unique ID of the client
    uint64_t _id;

    KeyboardInterface _keyboard;

    // Window parameters
    sf::RenderWindow _window;

    sf::Vector2u _resolution;
    std::string _window_name;
    bool _is_window_focused;
    bool _is_window_opened;

    // Params of current scene
    uint32_t _current_num_of_objects;
    std::vector<SpriteDrawer> _graph_objects;
    std::map<unsigned int, sf::Texture> _textures;

    // Object with all information about level
    Level _level;

    // Map properties
    sf::Vector2i _tile_size;
    float _map_width, _map_height;
    std::vector<Layer> _map_tile_layers;

    // Number of graph properties of all objects
    int _number_of_graph_objects;
};

uint64_t make_long_long(uint32_t first_bits, uint32_t last_bits);

sf::Packet &operator>>(sf::Packet &packet, uint64_t &number);

}
