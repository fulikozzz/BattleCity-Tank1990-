#include "Tank.h"
#include "Map.h"

Tank::Tank(String image_path, Position initPosition, Direction initDirection, int initLives, int initSpeed) :
	image_path(image_path), position(initPosition), direction(initDirection), lives(initLives), speed(initSpeed), delta(Position(0,0)) {	
	image.loadFromFile(image_path);
	image.createMaskFromColor(Color::White);
	texture.loadFromImage(image);
	sprite.setTexture(texture);
	sprite.setTextureRect(IntRect(0,0,54,54));
	sprite.setPosition(initPosition.getX(), initPosition.getY());
}

Position Tank::getPosition() { return position; }
Direction Tank::getDirection() { return direction; }
int Tank::getLives() { return lives; }
float Tank::getSpeed() { return speed; }
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
			//position.setY(position.getY() - speed);
			break;
		case RIGHT:
			sprite.setTextureRect(IntRect(162, 0, 54, 54));
			delta.setX(speed);
			delta.setY(0);
			//position.setX(position.getX() + speed);
			break;
		case DOWN:
			sprite.setTextureRect(IntRect(108, 0, 54, 54));
			delta.setY(speed);
			delta.setX(0);
			//position.setY(position.getY() + speed);
			break;
		case LEFT:
			sprite.setTextureRect(IntRect(54, 0, 54, 54));
			delta.setX(-speed);
			delta.setY(0);
			//position.setX(position.getX() - speed);
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