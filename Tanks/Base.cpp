#include "Base.h"


Base::Base() : position(0,0), isDestroyed(false) {}
Base::Base(Position initPosition, bool initIsDestroyed) : position(initPosition), isDestroyed(initIsDestroyed) {}

Position Base::getPositon() { return position; }
bool Base::getIsDestroyed() { return isDestroyed; }
void Base::setPosition(Position newPosition) { position = newPosition; }
void Base::setIsDestroyed(bool value) { isDestroyed = value; }