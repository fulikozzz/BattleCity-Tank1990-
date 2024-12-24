#include "Tank.h"
#include "Map.h"
#include <iostream>
Tank::Tank(String image_path, Position initPosition, Direction initDirection, int initLives, float initSpeed) :
	image_path(image_path), position(initPosition), direction(initDirection), lives(initLives), speed(initSpeed), delta(Position(0, 0)) {
	//Инициализация снаряда
	bullets.resize(AMOUNT_OF_BULLETS);
	for (int i = 0; i < AMOUNT_OF_BULLETS; i++) {
		bullets[i] = Bullet(Position(0, 0), UP, 0.2);
		//Для отрисовки снаряда
		bullets[i].setPath();
		bullets[i].setTextures();
		bullets[i].setSprite();
		bullets[i].getSprite().setPosition(0, 0);
	}
	//Для отрисовки игрока
	image.loadFromFile(image_path);
	image.createMaskFromColor(Color::White);
	texture.loadFromImage(image);
	sprite.setTexture(texture);
	sprite.setTextureRect(IntRect(0, 0, 54, 54));
	sprite.setPosition(initPosition.getX(), initPosition.getY());
}

Position Tank::getPosition() { return position; }
Direction Tank::getDirection() { return direction; }
int Tank::getLives() { return lives; }
float Tank::getSpeed() { return speed; }
vector<Bullet>& Tank::getBullets() { return bullets; }
Sprite Tank::getSprite() { return sprite; }
void Tank::setPosition(Position newPosition) { position = newPosition; }
void Tank::setDirection(Direction newDirection) { direction = newDirection; }
void Tank::setLives(int value) { lives = value; }
void Tank::setSpeed(float value) { speed = value; }

void Tank::move(float time) {
	if (!checkBoarderCollision(position.getX(), position.getY(), direction, speed, time)) {
		switch (direction)
		{
		case UP:
			sprite.setTextureRect(IntRect(0, 0, 54, 54));
			delta.setY(-speed);
			delta.setX(0);
			break;
		case RIGHT:
			sprite.setTextureRect(IntRect(162, 0, 54, 54));
			delta.setX(speed);
			delta.setY(0);
			break;
		case DOWN:
			sprite.setTextureRect(IntRect(108, 0, 54, 54));
			delta.setY(speed);
			delta.setX(0);
			break;
		case LEFT:
			sprite.setTextureRect(IntRect(54, 0, 54, 54));
			delta.setX(-speed);
			delta.setY(0);
			break;
		default:
			break;
		}
		position.setX(position.getX() + delta.getX() * time);
		position.setY(position.getY() + delta.getY() * time);
		speed = 0; 
		sprite.setPosition(position.getX(), position.getY());
	}
}

bool Tank::checkBoarderCollision(int currentX, int currentY, Direction currentDirection, float speed, float time) {
	Position newPosition = Position(currentX, currentY);
	switch (currentDirection)
	{
	case UP:
		newPosition.setY(currentY - speed * time);
		break;
	case RIGHT:
		newPosition.setX(currentX + speed * time);
		break;
	case DOWN:
		newPosition.setY(currentY + speed * time);
		break;
	case LEFT:
		newPosition.setX(currentX - speed * time);
		break;
	}
	
	if (newPosition.getX() < 0 || newPosition.getX() > 1026 || newPosition.getY() < 0 || newPosition.getY() > 1026)
		return true;
	else return false;
}

void Tank::shoot(float time) {
	// Найти первый неактивный снаряд
	auto it = find_if(getBullets().begin(), getBullets().end(), [](Bullet& bullet) {
		return !bullet.getIsActive(); // Найти неактивный снаряд
		});

	// Если найден неактивный снаряд
	if (it != getBullets().end()) {
		it->setIsActive(true);  // Активировать снаряд
		it->setDirection(this->getDirection());  // Установить направление

		// Установить позицию снаряда в зависимости от направления
		if (getDirection() == UP)
			it->setPosition(Position(this->getPosition().getX() + 18, this->getPosition().getY() - 9));
		else if (getDirection() == LEFT)
			it->setPosition(Position(this->getPosition().getX() - 9, this->getPosition().getY() + 18));
		else if (getDirection() == DOWN)
			it->setPosition(Position(this->getPosition().getX() + 18, this->getPosition().getY() + 45));
		else if (getDirection() == RIGHT)
			it->setPosition(Position(this->getPosition().getX() + 45, this->getPosition().getY() + 18));
	}
}