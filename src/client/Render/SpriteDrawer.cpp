#include "SpriteDrawer.h"

namespace cli
{

SpriteDrawer::SpriteDrawer(const sf::Texture &texture,
                           uint32_t width,
                           uint32_t height,
                           sf::Packet &packet) :
    _texture(texture),
    _width(width),
    _height(height)
{
    set_state_form_packet(packet);
    _sprite.setTexture(_texture);
}


void SpriteDrawer::set_state_form_packet(sf::Packet &pack)
{
    float x = 0, y = 0;
    uint32_t texture_id = 0, rotation = 0, current_frame = 0, direction = 0;
    pack >> x >> y >> texture_id >> direction >> rotation >> current_frame;
    if (direction == conf::game::Rest)
        current_frame = 0;

    _sprite.setTextureRect(sf::IntRect(_width * current_frame,
                                       2 * rotation * _height,
                                       _width,
                                       _height));
    _sprite.setPosition(x, y);
    set_position({x, y});
}


void SpriteDrawer::draw(sf::RenderWindow &window)
{
    window.draw(_sprite);
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
