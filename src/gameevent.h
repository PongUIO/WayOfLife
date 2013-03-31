#ifndef GAMEEVENT_H
#define GAMEEVENT_H
#include "gamedefines.h"

class GameEvent
{
	
public:
	GameEvent(EventType, int x, int y);
	virtual ~GameEvent();
	int mX, mY;
	EventType mType;
};

#endif // GAMEEVENT_H
