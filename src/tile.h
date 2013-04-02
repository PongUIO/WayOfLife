#ifndef TILE_H
#define TILE_H
#include "gamedefines.h"
#include<iostream>

class GameEngine;

class Tile
{
	
public:
	Tile(GameEngine *engine, int x, int y);
	virtual ~Tile();
	State getState() { return mState; }
	void setState(State v) { mState = v; }
	State getStoreState() { return mStoreState; }
	void setStoreState(State v) { mStoreState = v; }
	void assignStoredState() { mState = mStoreState; }
	SpecialEffect getSpecialEffect() { return mEffect; }
	void setSpecialEffect(SpecialEffect v) { mEffect = v; mInheritedEffect = mEffect; }
	SpecialEffect getInheritedSpecialEffect() { return mInheritedEffect; }
	void setInheritedSpecialEffect(SpecialEffect v) { if (mEffect == NONE) { mInheritedEffect = v; } }
	SpecialEffect getStoreEffect() { return mStoreEffect; }
	void setStoreEffect(SpecialEffect v) { mStoreEffect = v; mStoreEffectChanges++; }
	void assignStoredEffect();
	TileType getTileType() { return mType; }
	void setTileType(TileType v) { mType = v; }
	void calcAliveState();
	bool getDone() { return mDone; }
	void setDone(bool v) { mDone = v; }

private:
	State mState, mStoreState;
	TileType mType;
	SpecialEffect mEffect, mInheritedEffect, mStoreEffect;
	int mX, mY, mStoreEffectChanges;
	GameEngine *mEngine;
	bool mDone;
	
};

#endif // TILE_H
