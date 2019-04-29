#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_set>

#include "KeyboardInterface.h"
#include "../Render/SpriteDrawer.h"
#include "../../configuration/config.h"

namespace cli
{

class Manager
{

 public:

    Manager(uint32_t x_resolution, uint32_t y_resolution, std::string &&window_name);

    void set_remote_ip_port(std::pair<uint32_t, uint32_t> ip_port);

    int update(sf::Packet &packet);

    sf::Packet get_current_state();

    bool is_window_active();

    void load_textures();

    void activate();

    void draw();

    ~Manager() = default;

    sf::View _view;

 private:

    uint32_t _local_port, _local_ip;

    // Window parameters
    sf::RenderWindow _window;

    sf::Vector2u _resolution;
    std::string _window_name;
    bool _is_window_focused;
    bool _is_window_opened;

    KeyboardInterface _keyboard;

    uint32_t _current_num_of_objects;
    std::vector<SpriteDrawer> _graph_objects;
    std::map<unsigned int, sf::Texture> _textures;

    int process_scene(sf::Packet &packet);
};

}
