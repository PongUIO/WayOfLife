#ifndef TILE_H
#define TILE_H
#include "gamedefines.h"

class Tile
{
	
public:
	Tile();
	virtual ~Tile();
	CellState getCellState() { return mState; }
	void setCellState(CellState v) { mState = v; }
	TileType getTileType() { return mType; }
	void setTileType(TileType v) { mType = v; }

private:
	CellState mState;
	TileType mType;
	
};

#endif // TILE_H
