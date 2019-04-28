#pragma once
#include <iostream>
#include <SFML/Network.hpp>

namespace conf
{

namespace net
{

static const uint32_t REQUIRED_NUM_OF_CLIENTS = 2;
static const uint32_t CONNECTION_DELAY = 25;
static const uint16_t DEFAULT_PORT = 8080;
}

namespace game
{

static const uint64_t START_NUM_OF_OBJECTS = 1000;

static const float dude_animation_speed = 0.035;
static const int dude_frame_amount = 8;
static const int dude_width = 96;
static const int dude_height = 96;

static const float bullet_animation_speed = 0.035;
static const int bullet_frame_amount = 8;
static const int bullet_width = 64;
static const int bullet_height = 64;

enum Directions
{
    Up,
    Right,
    Down,
    Left,
    Rest,
};

enum ObjectTypes
{
    Empty,
    Player,
    Bullet,
};

enum Properties
{
    Rectangle,
    MatrixSprite,
};

enum Textures
{
    DudeTexture,
    BulletTexture,
};

}

}
