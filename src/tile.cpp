#include "tile.h"
#include "gameengine.h"

Tile::Tile(GameEngine *engine, int x, int y)
{
	mEngine = engine;
	mType = NORMNEIGH;
	mState = mStoreState = EMPTY;
	mEffect = NONE;
	mX = x;
	mY = y;
}

void Tile::calcAliveState()
{
	uint count = 0;
	for (int x = -1; x <= 1; x++) {
		for (int y = -1; y <= 1; y++) {
			if ((x == 0 && y == 0) || (mType == DIANEIGH && x * y == 0) || (mType == STRAIGHTNEIGH && x*y != 0)) {
				continue;
			}
			Tile *t = mEngine->getCellState(mX+x, mY+y);
			if (t != NULL && t->getCellState() == ALIVE) {
				count++;
			}
		}
	}
	if (count == 3 || (count == 2 && mState == ALIVE)) {
		mStoreState = ALIVE;
	} else {
		mStoreState = EMPTY;
	}
}


Tile::~Tile()
{

}