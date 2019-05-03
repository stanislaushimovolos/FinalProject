#include "SolidBlock.h"

namespace ser
{

SolidBlock::SolidBlock(Object &map_object) :
    GameObject({map_object.rect.left, map_object.rect.top}, {0, 0}, conf::game::Down,
               conf::game::Down, 0, conf::game::SolidBlock),

    _width(map_object.rect.width),
    _height(map_object.rect.height)
{
    _collider.set_size({_width,
                        _height});
    _collider.set_position(_position);

}


void SolidBlock::interact(ser::GameObject *object, int delta_t)
{

}


void SolidBlock::compress_to_packet(sf::Packet &pack) const
{
    pack << _position.x << _position.y << (uint32_t) _properties.size();
    compress_properties_to_packet(pack);
}

}
