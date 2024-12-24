#pragma once
#include "Position.h"
#include "Direction.h"
#include "Bullet.h"

#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>

using namespace sf;
using namespace std;

class Tank
{
private:
	Position position;
	Direction direction;
	int lives;
	float speed;
	Bullet bullet;

	// Для отрисовки
	Position delta;
	String image_path;
	Image image;
	Texture texture;
	Sprite sprite;

public:
	Tank(String image_path, Position initPosition, Direction initDirection, int initLives, float initSpeed);

	Position getPosition();
	Direction getDirection();
	int getLives();
	float getSpeed();
	Bullet& getBullets();
	Sprite getSprite();
	void setPosition(Position newPosition);
	void setDirection(Direction newDirection);
	void setLives(int value);
	void setSpeed(float value);

	void move(float time);
	void shoot(float time);
	
	bool checkBoarderCollision(int currentX, int currentY, Direction currentDirection, float speed, float time);
};

