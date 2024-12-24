#include "Enemy.h"

#include <random>

Enemy::Enemy(Position initPosition, Direction initDirection, int initLives, float initSpeed, int initArmor) :
    Tank("textures/Player_tank_sprite.png", initPosition, initDirection, initLives, initSpeed), armor(initArmor),
    randomMovementTime(0.0f), isRandomMovement(false) {}

int Enemy::getArmor() { return armor; }
void Enemy::setArmor(int value) { armor = value; }

void Enemy::startRandomMovement() {
    isRandomMovement = true;
    randomMovementTime = 800.0f;
    srand(static_cast <unsigned> (time(NULL)));
    random_device rd;   // non-deterministic generator
    mt19937 gen(rd());  // to seed mersenne twister.
    uniform_int_distribution<> dist(0, 3); // distribute results between 1 and 6 inclusive.
    setDirection(static_cast<Direction>(dist(gen)));
}

void Enemy::enemy_control(float time) {
    shoot(time);
    if (isRandomMovement) {
        randomMovementTime -= time;

        setSpeed(0.1);
        move(time);

        if (randomMovementTime <= 0) {
            isRandomMovement = false;
        }
    }

    if (!isRandomMovement) {
        bool incorrectPos = false;
        if (getPosition().getX() < pBaseX) {
            setDirection(RIGHT);
            incorrectPos = true;
        }
        else if (getPosition().getY() < pBaseY) {
            setDirection(DOWN);
            incorrectPos = true;
        }
        else if (getPosition().getX() > pBaseX) {
            setDirection(LEFT);
            incorrectPos = true;
        }
        else if (getPosition().getY() > pBaseY) {
            setDirection(UP);
            incorrectPos = true;
        }


        if (incorrectPos) {
            setSpeed(0.1);
            switch (getDirection()) {
            case UP: move(time); break;
            case LEFT: move(time); break;
            case DOWN: move(time); break;
            case RIGHT: move(time); break;
            default: break;
            }
        }
    }
}
