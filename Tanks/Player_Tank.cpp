#include "Player_Tank.h"
#include "SFML/Audio.hpp"
#include <iostream>

Player_Tank::Player_Tank(Position initPosition, Direction initDirection, int initLives, float initSpeed, int initArmor) :
	Tank("textures/Player_tank_sprite.png", initPosition, initDirection, initLives, initSpeed), score(0), armor(initArmor) {
	setShootSound("audio/shoot_standart.wav");
	engineBuffer.loadFromFile("audio/move_music.wav");
	engineSound.setBuffer(engineBuffer);
	engineSound.setVolume(7);
	idleBuffer.loadFromFile("audio/engine_music.wav");
	idleSound.setBuffer(idleBuffer);
	idleSound.setVolume(5);
}

int Player_Tank::getScore() { return score; }
int Player_Tank::getArmor() { return armor; }
Sound& Player_Tank::getEngineSound() { return engineSound; }
Sound& Player_Tank::getIdleSound() { return idleSound; }
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
	if (Keyboard::isKeyPressed(Keyboard::F)) {
		shoot(time);
	}

	if (is_Moving) {
		//setSpeed(0.1);
		if (engineSound.getStatus() != sf::Sound::Playing) {
			engineSound.play();
		}
		idleSound.stop();

		switch (getDirection()) {
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

	else {
		engineSound.stop();  // Останавливаем звук двигателя
		if (idleSound.getStatus() != sf::Sound::Playing) {
			idleSound.play();
		}
		
	}
}