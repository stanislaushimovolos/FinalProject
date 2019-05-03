#pragma once

#include "Player.h"
#include "Bullet.h"
#include "../Engine/GameObject.h"
#include "../../configuration/render.h"

namespace ser
{

class SolidBlock : public GameObject
{
 public:

    explicit SolidBlock(Object &map_object);

    void compress_to_packet(sf::Packet &pack) const override;

    void interact(GameObject *object, int delta_t) override;

    ~SolidBlock() override = default;

 private:

    float _width, _height;

};

}

