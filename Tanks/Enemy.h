#include "Tank.h"
#include "Base.h"

#pragma once

class Enemy : public Tank
{
private:
	int armor;
	// Для случайного движения
	float randomMovementTime;
	bool isRandomMovement;

public:
	Enemy(Position initPosition, Direction initDirection, int initLives, float initSpeed, int initArmor);
	Enemy(const Enemy& other) = default;
	Enemy& operator=(const Enemy& other);

	int getArmor();
	void setArmor(int value);

	void enemy_control(float time, Base& pBase);
	void startRandomMovement();
};

