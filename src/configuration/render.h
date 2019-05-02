#pragma once

#include <iostream>


namespace conf
{

namespace render
{

enum Textures
{
    DevilTexture,
    BulletTexture,
    GirlTexture,
    BoyTexture,
    GhostTexture
};

// Path to texture files
static const char boy_texture_relative_path[] = "../client/Textures/Boy.png";
static const char girl_texture_relative_path[] = "../client/Textures/Girl.png";
static const char devil_texture_relative_path[] = "../client/Textures/Devil.png";
static const char ghost_texture_relative_path[] = "../client/Textures/Ghost.png";
static const char fire_ball_texture_relative_path[] = "../client/Textures/FireBall.png";

/*
 * Parameters of animation. Each texture is described by its width, height,
 * animation speed and number of frames in one direction of movement.
 */

// Devil
static const int devil_frame_amount = 8;
static const float devil_texture_width = 96;
static const float dude_texture_height = 96;
static const float devil_animation_speed = 0.035;

// Bullet
static const int bullet_frame_amount = 8;
static const float bullet_texture_width = 64;
static const float bullet_texture_height = 64;
static const float bullet_animation_speed = 0.035;

// Girl
static const int girl_frame_amount = 6;
static const float girl_texture_width = 64;
static const float girl_texture_height = 90.875;
static const float girl_animation_speed = 0.017;

// Boy
static const int boy_frame_amount = 8;
static const float boy_texture_width = 100;
static const float boy_texture_height = 100;
static const float boy_animation_speed = 0.02;

// Ghost
static const int ghost_frame_amount = 8;
static const float ghost_texture_width = 100;
static const float ghost_texture_height = 100;
static const float ghost_animation_speed = 0.02;

}

}
