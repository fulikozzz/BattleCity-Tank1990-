#pragma once
#include "Position.h"
#include "Direction.h"

#include <SFML/Graphics.hpp>
using namespace sf;

class Bullet
{
private:
	Position position;
	Direction direction;
	float speed;

	// Для отрисовки
	Position delta;
	String image_path;
	Image image;
	Texture texture;
	Sprite sprite;
	
public:
	Bullet(Position initPosition, Direction initDirection, float initSpeed);

	Position getPosition();
	Direction getDirection();
	float getSpeed();
	void setPosition(Position newPosition);
	void setDirection(Direction newDirection);
	void setSpeed(float value);

	void move(float time);

	bool checkBoarderCollision(int currentX, int currentY, Direction currentDirection, float speed, float time);


	Sprite getSprite();
};

