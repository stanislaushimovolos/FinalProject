#include "Blast.h"

namespace ser
{

Blast::Blast(uint64_t owner, sf::Vector2f position, float damage) :
    GameObject(position, {0, 0}, conf::game::Down, conf::game::Down, 0, conf::game::Blast),
    _caused_damage(damage),
    _owner(owner)
{
    using namespace conf::render;

    add_property(conf::game::MainObjectSprite,
                 new MatrixSprite(this,
                                  BlastTexture,
                                  blast_animation_speed,
                                  blast_texture_width,
                                  blast_texture_height,
                                  blast_frame_amount));


    _life_time = blast_frame_amount / blast_animation_speed;
    _collider.set_size({blast_texture_width,
                        blast_texture_height});
    _collider.set_position({_position.x - blast_texture_width / 2,
                            _position.y - blast_texture_height / 2});
}


void Blast::compress_to_packet(sf::Packet &pack) const
{
    pack << _position.x << _position.y << (uint32_t) _properties.size();
    compress_properties_to_packet(pack);
}


void Blast::update(int delta_t)
{
    GameObject::update(delta_t);
    _life_time -= delta_t;
    if (_life_time <= 0)
        set_status(false);
}


void Blast::interact(ser::GameObject *object, int delta_t)
{
    auto other_type = object->get_type();
    if (!object->is_active())
        return;

    switch (other_type)
    {
        case (conf::game::Player):
        {
            const auto &other_collider = object->get_collider();
            if (!this->_collider.detect_collision(other_collider))
                return;

            auto player_ptr = dynamic_cast<Player *>(object);
            player_ptr->cause_damage(_caused_damage);
            break;
        }
        default:break;
    }
}


float Blast::get_damage() const
{
    return _caused_damage;
}

}
