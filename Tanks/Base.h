#pragma once
#include "Position.h"

class Base
{
private:
	Position position;
	bool isDestroyed;

public:
	Base();
	Base(Position position, bool isDestroyed);
	
	Position getPositon();
	bool getIsDestroyed();
	void setPosition(Position newPosition);
	void setIsDestroyed(bool value);

};

