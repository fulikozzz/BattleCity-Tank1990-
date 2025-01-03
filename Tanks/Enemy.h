#include "Tank.h"

#pragma once

class Enemy : public Tank
{
private:
	int armor;

	int pBaseX = 10 * 54;
	int pBaseY = 18 * 54;
	// ��� ���������� ��������
	float randomMovementTime;
	bool isRandomMovement;

public:
	Enemy(Position initPosition, Direction initDirection, int initLives, float initSpeed, int initArmor);
	Enemy(const Enemy& other) = default;
	Enemy& operator=(const Enemy& other);

	int getArmor();
	void setArmor(int value);

	void enemy_control(float time);

	void startRandomMovement();
};

