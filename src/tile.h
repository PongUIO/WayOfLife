#ifndef TILE_H
#define TILE_H
#include "gamedefines.h"

class GameEngine;

class Tile
{
	
public:
	Tile(GameEngine *engine, int x, int y);
	virtual ~Tile();
	CellState getCellState() { return mState; }
	void setCellState(CellState v) { mState = v; }
	CellState getCellStoreState() { return mStoreState; }
	void setCellStoreState(CellState v) { mStoreState = v; }
	void assignStoredState() { mState = mStoreState; }
	TileType getTileType() { return mType; }
	void setTileType(TileType v) { mType = v; }
	void calcAliveState();

private:
	CellState mState, mStoreState;
	TileType mType;
	int mX, mY;
	GameEngine *mEngine;
	
};

#endif // TILE_H
