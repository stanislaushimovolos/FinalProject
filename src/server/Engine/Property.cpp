#include "Property.h"

namespace ser
{

Property::Property(GameObject *master, uint32_t type) :
    _master(master),
    _type(type),
    _position(master->get_position())
{}


void Property::set_position(sf::Vector2f &pos)
{
    _position = pos;
}


uint32_t Property::get_type()
{
    return _type;
}


SimpleRectangleTexture::SimpleRectangleTexture(ser::GameObject *master,
                                               sf::Color color,
                                               sf::Vector2f &&shape) :
    Property(master, conf::game::Rectangle),
    _color(color.toInteger()),
    _shape(shape)
{}


void SimpleRectangleTexture::update(int delta_t)
{
    _position = _master->get_position();
}


void SimpleRectangleTexture::compress_to_packet(sf::Packet &pack) const
{
    pack << _color << _position.x << _position.y << _shape.x << _shape.y;
}

}
