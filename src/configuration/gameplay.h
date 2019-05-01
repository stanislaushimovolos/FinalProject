#pragma once

namespace conf
{

namespace game
{

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
};

// Type of textures (Rectangle is removed now)
enum Properties
{
    Rectangle,
    MatrixSprite,
};

}
}