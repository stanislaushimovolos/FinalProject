#include "SpriteDrawer.h"

namespace cli
{

SpriteDrawer::SpriteDrawer(std::map<unsigned int, sf::Texture> *textures) :
    _textures(textures),
    _rotation(conf::game::Empty),
    _width(0),
    _height(0),
    _texture_id(0),
    _current_frame_number(0),
    _fill_color(sf::Color::Transparent.toInteger())
{}


SpriteDrawer::SpriteDrawer() :
    SpriteDrawer(nullptr)
{}


/**
 * Change current texture by texture from packet.
 * @param pack packet with information about new texture.
 */
void SpriteDrawer::set_state_form_packet(sf::Packet &pack)
{
    float x = 0, y = 0;
    uint32_t texture_id = 0, rotation = 0,
        current_frame = 0, direction = 0;

    pack >> x >> y;
    pack >> texture_id;
    pack >> _fill_color;
    pack >> direction >> rotation;
    pack >> _width >> _height;
    pack >> current_frame;

    if (direction == conf::game::Rest)
        current_frame = 0;

    set_position({x, y});
    _sprite.setPosition(x, y);
    _sprite.setTextureRect(sf::IntRect(_width * current_frame,
                                       rotation * _height,
                                       _width,
                                       _height));

    _sprite.setColor(sf::Color(_fill_color));

    // Check if texture exists
    if ((*_textures).count(texture_id))
        _sprite.setTexture((*_textures)[texture_id]);
}


void SpriteDrawer::draw(sf::RenderWindow &window) const
{
    window.draw(_sprite);
}


void SpriteDrawer::set_texture_map(std::map<unsigned int, sf::Texture> *textures)
{
    _textures = textures;
}


void SpriteDrawer::set_position(sf::Vector2f &pos)
{
    _position = pos;
}


void SpriteDrawer::set_position(sf::Vector2f &&pos)
{
    _position = pos;
}


sf::Vector2f SpriteDrawer::get_position() const
{
    return _position;
}

}
