#include "Position.h"

Position::Position() : x(0), y(0) {}

Position::Position(float x, float y) : x(x), y(y) {}

Position::Position(const Position& other) : x(other.x), y(other.y) {}

float Position::getX() { return x; }

float Position::getY() { return y; }
void Position::setX(float value) { x = value; }
void Position::setY(float value) { y = value; }
void Position::setPosition(float newX, float newY) { x = newX;	y = newY; }