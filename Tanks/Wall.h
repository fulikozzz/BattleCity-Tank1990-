#pragma once
#include "Position.h"
#include <SFML/Graphics.hpp>

enum WallType {
    Empty = 0,
    IronWall = 1,
    WoodenWall = 2,
    BrickWallHigh = 3,
    BrickWallHalf = 4,
    BrickWallLow = 5,
    Tree = 6,
    Ice = 7,
    Water = 8,
    Player_Base = 9,
    Enemy_Base = 10
};

class Wall
{
private:
    Position position;
    WallType type;

public:
    Wall();
    Wall(Position position, WallType type);

    Position getPosition();
    WallType getType();

    void setPosition(Position newPosition);
    void setPosition(int newX, int newY);
    void setType(WallType newType);
};

