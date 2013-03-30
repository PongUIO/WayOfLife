#include "tile.h"
#include "gameengine.h"

Tile::Tile(GameEngine *engine, int x, int y)
{
	mStoreEffectChanges = 0;
	mEngine = engine;
	mType = NORMNEIGH;
	mState = mStoreState = EMPTY;
	mEffect = mInheritedEffect = mStoreEffect = NONE;
	mX = x;
	mY = y;
	mDone = false;
}

void Tile::calcAliveState()
{
	if (mState == SOLID) {
		return;
	}
	if (mInheritedEffect != NONE && mState == ALIVE) {
		if (mState == ALIVE) {
			int tx = mX - (mInheritedEffect == MOVLEFT) + (mInheritedEffect == MOVRIGHT);
			int ty = mY - (mInheritedEffect == MOVDOWN) + (mInheritedEffect == MOVUP);
			Tile *target = mEngine->getTile(tx, ty);
			if (target == NULL) {
				mStoreState = ALIVE;
				mStoreEffect = mEffect;
			} else {
				if (target->getState() == EMPTY || target->getInheritedSpecialEffect() != NONE) {
					target->setStoreState(ALIVE);
					target->setStoreEffect(mInheritedEffect);
				} else {
				}
			}
		} else {
		}
	} else {
		uint count = 0;
		for (int x = -1; x <= 1; x++) {
			for (int y = -1; y <= 1; y++) {
				if ((x == 0 && y == 0) || (mType == DIANEIGH && x * y == 0) || (mType == STRAIGHTNEIGH && x*y != 0)) {
					continue;
				}
				Tile *t = mEngine->getTile(mX+x, mY+y);
				if (t != NULL && t->getState() == ALIVE) {
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
}

void Tile::assignStoredEffect()
{
	if (mStoreEffectChanges <= 1 && mEffect == NONE) {
		mInheritedEffect = mStoreEffect;
	} else {
		mInheritedEffect = mEffect;
	}
	mStoreEffect = mEffect;
	mStoreEffectChanges = 0;
}



Tile::~Tile()
{

}