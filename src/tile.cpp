#include "tile.h"
#include "gameengine.h"

Tile::Tile(GameEngine *engine, uint x, uint y)
{
	mStoreEffectChanges = 0;
	mEngine = engine;
	mType = NORMNEIGH;
	mState = mStoreState = EMPTY;
	mEffect = mInheritedEffect = mStoreEffect = NONE;
	mX = x;
	mY = y;
	mDone = mDoneLoading = false;
	mLoaded = 0;
}

void Tile::calcAliveState()
{
	if (mState == SOLID || mEffect == AIR) {
		return;
	}
	if (mInheritedEffect != NONE && mState == ALIVE) {
		if (mState == ALIVE) {
			int tx = mX - (mInheritedEffect == MOVLEFT) + (mInheritedEffect == MOVRIGHT);
			int ty = mY - (mInheritedEffect == MOVDOWN) + (mInheritedEffect == MOVUP);
			Tile *target = mEngine->getTile(tx, ty);
			if (target != NULL && (target->getState() == EMPTY || target->getInheritedSpecialEffect() != NONE)) {
				target->setStoreState(ALIVE);
				target->setStoreEffect(mInheritedEffect);
			} else {
				mEngine->getEventMan().addEvent(GameEvent(SLOWDOWN, mX, mY));
				mStoreState = ALIVE;
				mStoreEffect = mEffect;
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
				if (t != NULL && (t->getState() == ALIVE || t->getState() == SOLID) && t->getInheritedSpecialEffect() == NONE) {
					count++;
				}
			}
		}
		if (count == 3 || (count == 2 && mState == ALIVE)) {
			if (mState != ALIVE) {
				if (mEffect != NONE) {
					mEngine->getEventMan().addEvent(GameEvent(SPEEDUP, mX, mY));
				} else {
					mEngine->getEventMan().addEvent(GameEvent(CREATION, mX, mY));
				}
			}
			mStoreState = ALIVE;
		} else {
			if (mState != EMPTY) {
				mEngine->getEventMan().addEvent(GameEvent(DELETION, mX, mY));
			}
			mStoreState = EMPTY;
		}
	}
}

void Tile::assignStoredEffect()
{
	if (mStoreEffectChanges <= 1 && mEffect == NONE) 
	{
		mInheritedEffect = mStoreEffect;
	} else 
	{
		mInheritedEffect = mEffect;
	}
	if (mStoreEffectChanges > 1) 
	{
		mState = mStoreState = SOLID;
		mEngine->getEventMan().addEvent(GameEvent(CRASH, mX, mY));
	}
	mStoreEffect = mEffect;
	mStoreEffectChanges = 0;
}

void Tile::updateLoaded(Ogre::Real dt)
{
	if (mDoneLoading) 
	{ 
		return;
	} 
	mLoaded = mLoaded-dt; 
	if (mLoaded < 0) 
	{ 
		mLoaded = 0; 
		mDoneLoading = true; 
	} 

}



Tile::~Tile()
{

}