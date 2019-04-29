#include "SpriteDrawer.h"

namespace cli
{

SpriteDrawer::SpriteDrawer(std::map<unsigned int, sf::Texture> *textures) :
    _textures(textures)
{}


void SpriteDrawer::set_state_form_packet(sf::Packet &pack)
{
    float x = 0, y = 0;
    uint32_t texture_id = 0, rotation = 0,
        current_frame = 0, direction = 0;

    pack >> x >> y;
    pack >> texture_id;
    pack >> direction >> rotation;
    pack >> _width >> _height;
    pack >> current_frame;

    if (direction == conf::game::Rest)
        current_frame = 0;

    set_position({x, y});
    _sprite.setPosition(x, y);
    _sprite.setTextureRect(sf::IntRect(_width * current_frame,
                                       2 * rotation * _height,
                                       _width,
                                       _height));

    switch (texture_id)
    {
        case (conf::game::DudeTexture):
        {
            _sprite.setTexture((*_textures)[conf::game::DudeTexture]);
            break;
        }
        case (conf::game::BulletTexture):
        {
            _sprite.setTexture((*_textures)[conf::game::BulletTexture]);
            break;
        }
        default:break;
    }
}


void SpriteDrawer::draw(sf::RenderWindow &window) const
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
