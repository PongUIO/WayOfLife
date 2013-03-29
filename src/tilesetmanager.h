#ifndef TILESETMANAGER_H
#define TILESETMANAGER_H
#include<OgrePrerequisites.h>
#include<vector>
#include<boost/random.hpp>

class TileSetManager
{
	
public:
	TileSetManager();
	virtual ~TileSetManager();
	void setActive(int act) { mActive = act; }
	int getActive() { return mActive; }
	int getActiveLen() { return mTileSets[mActive].size(); }
	Ogre::String getMaterial(int i) { return mTileSets[mActive][i]; }
	int getRandomActiveMaterial();
	boost::rand48 mRng;
protected:
	std::vector<std::vector<Ogre::String>> mTileSets;
	int mActive;
};

#endif // TILESETMANAGER_H
