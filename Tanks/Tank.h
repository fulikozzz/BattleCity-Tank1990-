#pragma once
#include "Position.h"
#include "Direction.h"
#include "Bullet.h"

#include "SFML/Audio.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include <chrono>

using namespace sf;
using namespace std;

#define AMOUNT_OF_BULLETS 2

class Tank
{
private:
	Position position;
	Direction direction;
	int lives;
	float speed;
	vector<Bullet> bullets;

	// Для отрисовки
	Position delta;
	String image_path;
	Image image;
	Texture texture;
	Sprite sprite;
	

	// Звуки для стрельбы 
	SoundBuffer shootBuffer;     
	Sound shootSound;

public:
	chrono::steady_clock::time_point lastShotTime;
	const chrono::milliseconds shotCooldown = std::chrono::milliseconds(700);

protected:
	void playShootSound();

public:
	Tank(String image_path, Position initPosition, Direction initDirection, int initLives, float initSpeed);
	Tank(const Tank& other);
	Tank& operator=(const Tank& other);

	Position getPosition();
	Direction getDirection();
	int getLives();
	float getSpeed();
	vector<Bullet>& getBullets();
	Sprite& getSprite();
	void setPosition(Position newPosition);
	void setDirection(Direction newDirection);
	void setLives(int value);
	void setSpeed(float value);
	void setShootSound(const std::string& filepath);

	void move(float time);
	void shoot(float time);
	
	bool checkBoarderCollision(int currentX, int currentY, Direction currentDirection, float speed, float time);
};

