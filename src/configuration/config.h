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

enum MoveDirections
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

// Type of textures (Rectangle is removed now)
enum Properties
{
    Rectangle,
    MatrixSprite,
};

enum Textures
{
    DevilTexture,
    BulletTexture,
};

static const uint64_t START_NUM_OF_OBJECTS = 100;

// Path to texture files
static const char devil_texture_relative_path[] = "../client/Textures/Devil.png";
static const char fire_ball_texture_relative_path[] = "../client/Textures/FireBall.png";


/*
 * Parameters of animation. Each texture is described by its width, height,
 * animation speed and number of frames in one direction of movement.
 */

// Devil
static const float devil_animation_speed = 0.035;
static const int devil_frame_amount = 8;
static const int devil_texture_width = 96;
static const int dude_texture_height = 96;

// Bullet
static const float bullet_animation_speed = 0.035;
static const int bullet_frame_amount = 8;
static const int bullet_texture_width = 64;
static const int bullet_texture_height = 64;

}

}
