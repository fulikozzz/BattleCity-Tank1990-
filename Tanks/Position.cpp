#include "Position.h"

Position::Position(float x, float y) {
	this->x = x;
	this->y = y;
}

float Position::getX() { return x; }

float Position::getY() { return y; }
void Position::setX(float value) { x = value; }
void Position::setY(float value) { y = value; }
void Position::setPosition(float newX, float newY) { x = newX;	y = newY; }