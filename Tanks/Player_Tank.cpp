#include "Player_Tank.h"

Player_Tank::Player_Tank(Position initPosition, Direction initDirection, int initLives, float initSpeed, int initArmor) :
	Tank("textures/Player_tank_sprite.png", initPosition, initDirection, initLives, initSpeed), score(0), armor(initArmor) {}

int Player_Tank::getScore() { return score; }
int Player_Tank::getArmor() { return armor; }
void Player_Tank::setScore(int value) { score = value; }
void Player_Tank::setArmor(int value) { armor = value; }

void Player_Tank::control(float time) {
	bool is_Moving = false;
	if (Keyboard::isKeyPressed(Keyboard::W)) {
		setDirection(UP);
		is_Moving = true;
	}
	if (Keyboard::isKeyPressed(Keyboard::A)) {
		setDirection(LEFT);
		is_Moving = true;
	}
	if (Keyboard::isKeyPressed(Keyboard::S)) {
		setDirection(DOWN);
		is_Moving = true;
	}
	if (Keyboard::isKeyPressed(Keyboard::D)) {
		setDirection(RIGHT);
		is_Moving = true;
	}
	if (is_Moving) {
		setSpeed(0.1);
		switch (getDirection())
		{
		case UP:
			move(time);
			break;
		case LEFT:
			move(time);
			break;
		case DOWN:
			move(time);
			break;
		case RIGHT:
			move(time);
			break;
		default:
			break;
		}
	}
}