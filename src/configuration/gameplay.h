#pragma once

namespace conf
{

namespace game
{

static const uint64_t start_num_of_objects = 100;

// Player parameters
static const float player_speed = 0.4;
static const float player_health_points = 500;

// Bullet parameters
static const float bullet_speed = 1.6;
static const float bullet_damage = 1.5;

// Black hole parameters
static const float hole_damage = 0.1;
static const float hole_speed = 1;

// Blast parameters
static const float blast_damage = 0.3;

static const uint32_t one_shoot_required_clicks = 7;

enum MoveDirections
{
    Down,
    Left,
    Right,
    Up,
    DownLeft,
    UpLeft,
    DownRight,
    UpRight,
    Rest = 228,
};

enum ObjectTypes
{
    Empty,
    Player,
    Bullet,
    Blast,
    MovingPlatform,
    SolidBlock
};

// Types of textures (Rectangle is removed now)
enum Properties
{
    Rectangle,
    MatrixSprite,
};

// Types of properties
enum PropertyKeys
{
    MainObjectSprite,
};

}

}