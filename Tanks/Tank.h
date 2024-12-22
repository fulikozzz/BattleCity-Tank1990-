#pragma once
#include "Position.h"
#include "Direction.h"

#include <SFML/Graphics.hpp>
using namespace sf;

class Tank
{
private:
	Position position;
	Direction direction;
	int lives;
	float speed;

	// Для отрисовки
	Position delta;
	String image_path;
	Image image;
	Texture texture;
	Sprite sprite;

public:
	Tank(String image_path, Position initPosition, Direction initDirection, int initLives, int initSpeed);

	Position getPosition();
	Direction getDirection();
	int getLives();
	float getSpeed();
	void setPosition(Position newPosition);
	void setDirection(Direction newDirection);
	void setLives(int value);
	void setSpeed(float value);

	void move(float time);

	bool checkBoarderCollision(int currentX, int currentY, Direction currentDirection, float speed, float time);


	Sprite getSprite() { return sprite; }
};

