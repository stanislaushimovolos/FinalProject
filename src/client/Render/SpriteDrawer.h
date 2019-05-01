#pragma once

#include <SFML/Graphics.hpp>
#include "../../configuration/config.h"

namespace cli
{


class SpriteDrawer
{
 public:

    explicit SpriteDrawer(std::map<unsigned int, sf::Texture> *textures);

    void set_texture_map(std::map<unsigned int, sf::Texture> *textures);

    void set_state_form_packet(sf::Packet &pack);

    void draw(sf::RenderWindow &window) const;

    void set_position(sf::Vector2f &&pos);

    void set_position(sf::Vector2f &pos);

    sf::Vector2f get_position() const;

    SpriteDrawer();

 private:

    float _width;
    float _height;

    uint32_t _fill_color;
    uint32_t _rotation;
    uint32_t _texture_id;
    uint32_t _current_frame_number;

    sf::Vector2f _position;
    sf::Sprite _sprite;

    std::map<unsigned int, sf::Texture> *_textures;
};

}
