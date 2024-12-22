#pragma once
#include "Tank.h"

class Player_Tank : public Tank
{
private:
	int score;
	int armor;

public:
	Player_Tank(Position initPosition, Direction initDirection, int initLives, float initSpeed, int armor);

	int getScore();
	int getArmor();
	void setScore(int value);
	void setArmor(int value);

	void control(float time);
};