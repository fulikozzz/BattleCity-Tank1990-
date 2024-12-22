#include "Bullet.h"
Bullet::Bullet(Position initPosition, Direction initDirection, float initSpeed) : position(initPosition), direction(initDirection), 
	speed(initSpeed), delta(Position(0,0)){

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
void Bullet::setPosition(Position newPosition) { position = newPosition; }
void Bullet::setDirection(Direction newDirection) { direction = newDirection; }
void Bullet::setSpeed(float value) { speed = value; }

void Bullet::move(float time){
	if (!checkBoarderCollision(position.getX(), position.getY(), direction, speed, time)) {
		switch (direction)
		{
		case UP:
			sprite.rotate(0);
			delta.setY(-speed);
			delta.setX(0);
			//position.setY(position.getY() - speed);
			break;
		case RIGHT:
			sprite.rotate(90 * 180 / 3.14);
			delta.setX(speed);
			delta.setY(0);
			//position.setX(position.getX() + speed);
			break;
		case DOWN:
			//sprite.rotate(180); 
			delta.setY(speed);
			delta.setX(0);
			//position.setY(position.getY() + speed);
			break;
		case LEFT:
			//sprite.rotate(270);
			delta.setX(-speed);
			delta.setY(0);
			//position.setX(position.getX() - speed);
			break;
		default:
			break;
		}
		position.setX(position.getX() + delta.getX() * time);
		position.setY(position.getY() + delta.getY() * time);
		sprite.setPosition(position.getX(), position.getY());
	}
}

bool Bullet::checkBoarderCollision(int currentX, int currentY, Direction currentDirection, float speed, float time){
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

	if (newPosition.getX() < 0 || newPosition.getX() > 1080-sprite.getGlobalBounds().width || newPosition.getY() < 0 || newPosition.getY() > 1080-sprite.getGlobalBounds().height)
		return true;
	else return false;
}


Sprite Bullet::getSprite() { return sprite; }