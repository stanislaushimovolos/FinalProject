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
    switch (type)
    {
        case conf::game::Player:
        {
            _texture.setSize(sf::Vector2f(40, 40));
            _texture.setFillColor(sf::Color::Red);
            break;
        }
        case conf::game::Bullet:
        {
            _texture.setSize(sf::Vector2f(4, 4));
            _texture.setFillColor(sf::Color::Yellow);
            break;
        }
        default:break;
    }
    set_state_form_packet(pack);
}


void SimpleRectangleDrawer::set_state_form_packet(sf::Packet &pack)
{
    float x, y = 0;
    pack >> x >> y;

    set_position({x, y});
    _texture.setPosition({x, y});
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
