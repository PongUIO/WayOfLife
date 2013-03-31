#include "gameevent.h"

GameEvent::GameEvent(EventType type, int x, int y)
{
	mType = type;
	mX = x*TILESIZE;
	mY = y*TILESIZE;
}

GameEvent::~GameEvent()
{

}

