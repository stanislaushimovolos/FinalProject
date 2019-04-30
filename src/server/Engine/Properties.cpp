#include "Properties.h"

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


MatrixSprite::MatrixSprite(ser::GameObject *master,
                           uint32_t texture_id,
                           float animation_speed,
                           float width,
                           float height,
                           uint32_t frame_amount) :

    Property(master, conf::game::MatrixSprite),
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
    _current_frame_number = ((uint32_t) _animation_timer) % _number_of_frames;
}


void MatrixSprite::compress_to_packet(sf::Packet &pack) const
{
    pack << _position.x - _width / 2
         << _position.y - _height / 2
         << _texture_id
         << _direction
         << _rotation
         << _width
         << _height
         << _current_frame_number;
}


RectCollider::RectCollider(sf::Vector2f &position, sf::Vector2f &size)
{
    _bounding_box.setSize(size);
    _bounding_box.setPosition(position);
    _bounding_box.setFillColor(sf::Color::Transparent);
}


bool RectCollider::detect_collision(const RectCollider &other_collider) const
{
    auto other_size = other_collider.get_size();
    auto other_position = other_collider.get_position();

    auto this_size = this->get_size();
    auto this_position = this->get_position();

    auto other_bottom = other_position.y + other_size.y;
    auto other_right = other_position.x + other_size.x;
    auto other_left = other_position.x;
    auto other_top = other_position.y;

    auto this_bottom = this_position.y + this_size.y;
    auto this_right = this_position.x + this_size.x;
    auto this_left = this_position.x;
    auto this_top = this_position.y;

    if (this_left > other_right ||
        this_right < other_left ||
        this_top > other_bottom ||
        this_bottom < other_top)
        return false;

    std::cout << "in collision" << std::endl;
    return true;
}


void RectCollider::set_position(const sf::Vector2f &new_position)
{
    _bounding_box.setPosition(new_position);
}


void RectCollider::set_size(const sf::Vector2f &new_size)
{
    _bounding_box.setSize(new_size);
}


sf::Vector2f RectCollider::get_position() const
{
    return _bounding_box.getPosition();
}


sf::Vector2f RectCollider::get_size() const
{
    return _bounding_box.getSize();
}

}
