#pragma once

#include <SFML/Graphics.hpp>
#include "../../configuration/config.h"

namespace cli
{


class SpriteDrawer
{
 public:

    explicit SpriteDrawer(const std::vector<sf::Texture> *textures);

    void set_state_form_packet(sf::Packet &pack);

    void draw(sf::RenderWindow &window) const;

    void set_position(sf::Vector2f &pos);

    void set_position(sf::Vector2f &&pos);

    sf::Vector2f get_position() const;

    const std::vector<sf::Texture> *_textures;

    SpriteDrawer() = default;

 private:

    uint32_t _width;
    uint32_t _height;

    uint32_t _rotation;
    uint32_t _texture_id;
    uint32_t _current_frame_number;

    sf::Vector2f _position;
    sf::Sprite _sprite;
};

}
