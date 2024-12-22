#include "Wall.h"
Wall::Wall() : position(Position(0, 0)), type(Empty) {}

Wall::Wall(Position position, WallType type) : position(position), type(type) {}

Position Wall::getPosition() { return position; }
WallType Wall::getType() { return type; }

void Wall::setPosition(Position newPosition) { position = newPosition; }
void Wall::setPosition(int newX, int newY) { position.setX(newX); position.setY(newY); }
void Wall::setType(WallType newType) { type = newType; }