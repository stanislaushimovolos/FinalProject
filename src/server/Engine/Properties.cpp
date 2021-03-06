#include "Properties.h"

namespace ser
{

GraphProperty::GraphProperty(GameObject *master, uint32_t type) :
    _master(master),
    _type(type),
    _position(master->get_position())
{}


void GraphProperty::set_position(sf::Vector2f &pos)
{
    _position = pos;
}


uint32_t GraphProperty::get_type()
{
    return _type;
}


SimpleRectangleTexture::SimpleRectangleTexture(ser::GameObject *master,
                                               sf::Color color,
                                               sf::Vector2f &&shape) :
    GraphProperty(master, conf::game::Rectangle),
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


MatrixSprite::MatrixSprite(ser::GameObject *master,
                           uint32_t texture_id,
                           float animation_speed,
                           float width,
                           float height,
                           uint32_t frame_amount) :

    GraphProperty(master, conf::game::MatrixSprite),
    _fill_color(sf::Color::White),
    _width(width),
    _height(height),
    _number_of_frames(frame_amount),
    _animation_speed(animation_speed),
    _texture_id(texture_id),
    _current_frame_number(0),
    _animation_timer(0),
    _rotation(conf::game::Rest)
{}


void MatrixSprite::update(int delta_t)
{
    _position = _master->get_position();
    _rotation = _master->get_rotation();
    _direction = _master->get_direction();

    if (_direction != conf::game::Rest)
        _animation_timer += _animation_speed * delta_t;

    // Compute number of new frame
    _current_frame_number = ((uint32_t) _animation_timer) % _number_of_frames;
}


void MatrixSprite::compress_to_packet(sf::Packet &pack) const
{
    pack << _position.x - _width / 2
         << _position.y - _height / 2
         << _texture_id
         << _fill_color.toInteger()
         << _direction
         << _rotation
         << _width
         << _height
         << _current_frame_number;
}


void MatrixSprite::set_color(sf::Color clr)
{
    _fill_color = clr;
}


void MatrixSprite::set_texture_id(uint32_t id)
{
    _texture_id = id;
}


}
