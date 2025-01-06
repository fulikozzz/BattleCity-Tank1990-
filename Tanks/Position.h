#pragma once
class Position
{
private:
	float x;
	float y;
	
public:
	Position();
	Position(float x, float y);
	Position(const Position& other);
	float getX();
	float getY();
	void setX(float value);
	void setY(float value);
	void setPosition(float newX, float newY);
};

