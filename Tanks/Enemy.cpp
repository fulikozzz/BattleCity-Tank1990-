#include "Enemy.h"

#include <random>

Enemy::Enemy(Position initPosition, Direction initDirection, int initLives, float initSpeed, int initArmor) :
    Tank("textures/enemy_tank_sprite.png", initPosition, initDirection, initLives, initSpeed), armor(initArmor),
    randomMovementTime(0.0f), isRandomMovement(false) {}

Enemy& Enemy::operator=(const Enemy& other) {
    if (this == &other) return *this;

    Tank::operator=(other);
    armor = other.armor;
    randomMovementTime = other.randomMovementTime;
    isRandomMovement = other.isRandomMovement;
    return *this;
}

int Enemy::getArmor() { return armor; }
void Enemy::setArmor(int value) { armor = value; }

void Enemy::startRandomMovement() {
    isRandomMovement = true;
    int someRand = rand() % 3000;
    someRand > 800.0f ? randomMovementTime = someRand : randomMovementTime = 800.0f;
    srand(static_cast <unsigned> (time(NULL)));
    random_device rd;  
    mt19937 gen(rd()); 
    uniform_int_distribution<> dist(0, 3); 
    setDirection(static_cast<Direction>(dist(gen)));
}

void Enemy::enemy_control(float time, Base& pBase) {
    if (armor == 0) {
        bool incorrectPos = false;

        if (!isRandomMovement) {
            if (getPosition().getX() > pBase.getPositon().getX()) {
                setDirection(LEFT);
                incorrectPos = true;
            }
            else if (getPosition().getY() < pBase.getPositon().getY()) {
                setDirection(DOWN);
                incorrectPos = true;
            }
            else if (getPosition().getX() < pBase.getPositon().getX()) {
                setDirection(RIGHT);
                incorrectPos = true;
            }
            else if (getPosition().getY() > pBase.getPositon().getY()) {
                setDirection(UP);
                incorrectPos = true;
            }

            if (incorrectPos) {
                switch (getDirection()) {
                case UP: move(time); break;
                case LEFT: move(time); break;
                case DOWN: move(time); break;
                case RIGHT: move(time); break;
                default: break;
                }
            }
        }
        else {
            randomMovementTime -= time;
            move(time);

            if (randomMovementTime <= 0) {
                isRandomMovement = false;
            }
        }
    }
    else if (armor == 1) {
        bool incorrectPos = false;
         if (!isRandomMovement) {
            if (getPosition().getX() < pBase.getPositon().getX()) {
                setDirection(RIGHT);
                incorrectPos = true;
            }
            else if (getPosition().getY() < pBase.getPositon().getY()) {
                setDirection(DOWN);
                incorrectPos = true;
            }
            else if (getPosition().getX() > pBase.getPositon().getX()) {
                setDirection(LEFT);
                incorrectPos = true;
            }
            else if (getPosition().getY() > pBase.getPositon().getY()) {
                setDirection(UP);
                incorrectPos = true;
            }

            if (incorrectPos) {
                switch (getDirection()) {
                case UP: move(time); break;
                case LEFT: move(time); break;
                case DOWN: move(time); break;
                case RIGHT: move(time); break;
                default: break;
                }
            }
        }

        else {
            randomMovementTime -= time;
            move(time);

            if (randomMovementTime <= 0) {
                isRandomMovement = false;
            }
        }
    }
    else if (armor == 3){
        bool incorrectPos = false;
        if (!isRandomMovement) {
            if (getPosition().getY() < pBase.getPositon().getY()) {
                setDirection(DOWN);
                incorrectPos = true;
            }
            else if (getPosition().getX() < pBase.getPositon().getX()) {
                setDirection(RIGHT);
                incorrectPos = true;
            }
            else if (getPosition().getX() > pBase.getPositon().getX()) {
                setDirection(LEFT);
                incorrectPos = true;
            }
            else if (getPosition().getY() > pBase.getPositon().getY()) {
                setDirection(UP);
                incorrectPos = true;
            }

            if (incorrectPos) {
                switch (getDirection()) {
                case UP: move(time); break;
                case LEFT: move(time); break;
                case DOWN: move(time); break;
                case RIGHT: move(time); break;
                default: break;
                }
            }
        }

        else {
            randomMovementTime -= time;
            move(time);

            if (randomMovementTime <= 0) {
                isRandomMovement = false;
            }
        }
    }
    else {
        bool incorrectPos = false;
        if (!isRandomMovement) {
            if (getPosition().getY() > pBase.getPositon().getY()) {
                setDirection(UP);
                incorrectPos = true;
            }
            else if (getPosition().getX() < pBase.getPositon().getX()) {
                setDirection(RIGHT);
                incorrectPos = true;
            }
            else if (getPosition().getX() > pBase.getPositon().getX()) {
                setDirection(LEFT);
                incorrectPos = true;
            }
            else if (getPosition().getY() < pBase.getPositon().getY()) {
                setDirection(DOWN);
                incorrectPos = true;
            }

            if (incorrectPos) {
                switch (getDirection()) {
                case UP: move(time); break;
                case LEFT: move(time); break;
                case DOWN: move(time); break;
                case RIGHT: move(time); break;
                default: break;
                }
            }
        }

        else {
            randomMovementTime -= time;
            move(time);

            if (randomMovementTime <= 0) {
                isRandomMovement = false;
            }
        }
    }

    int chance = rand() % 1000;  
    if (chance < 10) {           
        shoot(time);             
    }
}
