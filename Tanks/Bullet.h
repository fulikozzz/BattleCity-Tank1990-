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
	bool isActive;

	// Для отрисовки
	Position delta;
	String image_path;
	Image image;
	Texture texture;
	Sprite sprite;
	
public:
	Bullet();
	Bullet(Position initPosition, Direction initDirection, float initSpeed);

	Position getPosition();
	Direction getDirection();
	float getSpeed();
	bool getIsActive();
	void setPosition(Position newPosition);
	void setDirection(Direction newDirection);
	void setSpeed(float value);
	void setIsActive(bool value);
	
	void move(float time);

	bool checkBoarderCollision(float currentX, float currentY, Direction currentDirection, float speed, float time);

	//Для отрисовки
	Sprite getSprite();
	String getImagePath();
	Texture& getTexture();
	void setPath();
	void setTextures();
	void setSprite();
};

