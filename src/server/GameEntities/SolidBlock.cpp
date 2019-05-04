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
    auto other_type = object->get_type();
    if (!object->is_active())
        return;

    switch (other_type)
    {
        case (conf::game::Bullet):
        {
            // Don't do anything if there is no collision
            const auto &other_collider = object->get_collider();
            if (!this->_collider.detect_collision(other_collider))
                return;

            object->set_status(false);
            break;
        }
        default:break;
    }

}


void SolidBlock::compress_to_packet(sf::Packet &pack) const
{
    pack << _position.x << _position.y << (uint32_t) _properties.size();
}

}
