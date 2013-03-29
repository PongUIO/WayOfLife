#include "tilesetmanager.h"

TileSetManager::TileSetManager()
{
	mActive = 0;
	mTileSets.push_back(std::vector<Ogre::String>());
	mTileSets[0].push_back("rock1");
	mTileSets[0].push_back("rock2");
	mRng.seed();
}

int TileSetManager::getRandomActiveMaterial()
{
	return mRng() % mTileSets[mActive].size();
}


TileSetManager::~TileSetManager()
{

}

