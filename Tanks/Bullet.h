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

	// Для анимации взрыва
	bool isExploding;
	float explosionTimer;
	int currentExplosionFrame;

	// Для отрисовки
	Position delta;
	String image_path;
	Image image;
	String exp_image_path;
	Image exp_image;
	Texture texture;
	Sprite sprite;
	Texture explosionTexture;
	Sprite explosionSprite;
public:
	Bullet();
	Bullet(Position initPosition, Direction initDirection, float initSpeed);

	void startExplosion();
	void updateExplosion(float time);
	void renderExplosion(RenderWindow& window);
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
	Sprite getExpSprite();
	String getImagePath();
	String getExpImagePath();
	Texture& getTexture();
	Texture& getExpTexture();
	void setPath();
	void setExpPath();
	void setTextures();
	void setExpTextures();
	void setSprite();
	void setExpSprite();
};

