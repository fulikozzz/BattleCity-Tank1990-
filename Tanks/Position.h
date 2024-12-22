#pragma once
class Position
{
private:
	float x;
	float y;
	
public:
	Position(float x, float y);
	float getX();
	float getY();
	void setX(float value);
	void setY(float value);
	void setPosition(float newX, float newY);
};

