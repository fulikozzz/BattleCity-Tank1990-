#include "Bullet.h"
Bullet::Bullet() : position(Position(0, 0)), direction(UP), speed(0.3), isActive(false), delta(Position(0, 0)) {}

Bullet::Bullet(Position initPosition, Direction initDirection, float initSpeed) :
	position(initPosition), direction(initDirection), speed(initSpeed), isActive(false), delta(Position(0, 0)) {
	image_path = "textures/bullet_standard.png";
	image.loadFromFile(image_path);
	image.createMaskFromColor(Color::White);
	texture.loadFromImage(image);
	sprite.setTexture(texture);
	sprite.setPosition(initPosition.getX(), initPosition.getY());
}

Position Bullet::getPosition() { return position; }
Direction Bullet::getDirection() { return direction; }
float Bullet::getSpeed() { return speed; }
bool Bullet::getIsActive() { return isActive; }
void Bullet::setPosition(Position newPosition) { position = newPosition; }
void Bullet::setDirection(Direction newDirection) { direction = newDirection; }
void Bullet::setSpeed(float value) { speed = value; }
void Bullet::setIsActive(bool value) { isActive = value; }

void Bullet::move(float time) {
	if (isActive && !checkBoarderCollision(position.getX(), position.getY(), direction, speed, time)) {
		switch (direction)
		{
		case UP:
			delta.setY(-speed);
			delta.setX(0);
			break;
		case RIGHT:
			delta.setX(speed);
			delta.setY(0);
			break;
		case DOWN:
			delta.setY(speed);
			delta.setX(0);
			break;
		case LEFT:
			delta.setX(-speed);
			delta.setY(0);
			break;
		default:
			break;
		}
		position.setX(position.getX() + delta.getX() * time);
		position.setY(position.getY() + delta.getY() * time);
		sprite.setPosition(position.getX(), position.getY());
	}
	else isActive = false;
}

bool Bullet::checkBoarderCollision(float currentX, float currentY, Direction currentDirection, float speed, float time) {
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

	if (newPosition.getX() < 0 || newPosition.getX() > 1062 || newPosition.getY() < 0 || newPosition.getY() > 1062) {
		isActive = false;
		return true;
	}
	else return false;
}


Sprite Bullet::getSprite() { return sprite; }
String Bullet::getImagePath() { return image_path; }
Texture& Bullet::getTexture() { return texture; }
void Bullet::setPath() { image_path = "textures/bullet_standard.png"; image.loadFromFile(image_path); }
void Bullet::setTextures() { image.createMaskFromColor(Color::White); texture.loadFromImage(image); }
void Bullet::setSprite() { sprite.setTexture(texture); }
