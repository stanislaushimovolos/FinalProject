#include "SimpleRectangleDrawer.h"

namespace cli
{

void SimpleRectangleDrawer::draw(sf::RenderWindow &window)
{
    window.draw(_texture);
}


SimpleRectangleDrawer::SimpleRectangleDrawer(uint32_t type, sf::Packet &pack) :
    _type(type)
{
    set_state_form_packet(pack);
}


void SimpleRectangleDrawer::set_state_form_packet(sf::Packet &pack)
{
    float x = 0, y = 0, shape_x = 0, shape_y = 0;
    uint32_t color = 0;

    pack >> color >> x >> y >> shape_x >> shape_y;

    _texture.setSize({shape_x, shape_y});
    _texture.setPosition({x, y});
    _texture.setFillColor(sf::Color(color));

    set_position({x, y});
}


void SimpleRectangleDrawer::set_position(sf::Vector2f &pos)
{
    _position = pos;
}


void SimpleRectangleDrawer::set_position(sf::Vector2f &&pos)
{
    _position = pos;
}


sf::Vector2f SimpleRectangleDrawer::get_position() const
{
    return _position;
}

}
