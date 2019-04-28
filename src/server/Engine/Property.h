#pragma once

#include "GameObject.h"
#include <SFML/Graphics.hpp>

namespace ser
{

class GameObject;

class Property
{
 public:

    explicit Property(GameObject *master, uint32_t type);

    virtual void compress_to_packet(sf::Packet &pack) const = 0;

    virtual void update(int delta_t) = 0;

    void set_position(sf::Vector2f &pos);

    uint32_t get_type();

 protected:

    GameObject *_master;
    sf::Vector2f _position;
    uint32_t _type;
};

class SimpleRectangleTexture : public Property
{
 public:

    SimpleRectangleTexture(GameObject *master, sf::Color color, sf::Vector2f &&shape);

    void compress_to_packet(sf::Packet &pack) const override;

    void update(int delta_t) override;

 private:

    uint32_t _color;
    sf::Vector2f _shape;
};

class MatrixSprite : public Property
{
 public:

    MatrixSprite(GameObject *master,
                 uint32_t texture_id,
                 float animation_speed,
                 uint32_t width,
                 uint32_t height,
                 uint32_t frame_amount);

    void compress_to_packet(sf::Packet &pack) const override;

    void update(int delta_t) override;

 private:

    float _animation_speed;
    float _animation_timer;

    uint32_t _width;
    uint32_t _height;

    uint32_t _current_frame_number;
    uint32_t _number_of_frames;
    uint32_t _texture_id;
    uint32_t _rotation;
    uint32_t _direction;
};

}
