#ifndef GAMEENGINE_H
#define GAMEENGINE_H

class Creature;class GameEngine;
class Player;
#include<OgreSceneManager.h>
#include<OgreBillboardSet.h>
#include<OISKeyboard.h>
#include<OgreMovableObject.h>
#include<OgreTextAreaOverlayElement.h>
#include<OgreFontManager.h>
#include<OgreManualObject.h>
#include<algorithm>
#include<stdlib.h>
#include<OgreVector3.h>
#include <OgreResourceManager.h>
#include "tilesetmanager.h"
#include "billboarditem.h"
#include "../gorilla/Gorilla.h"
#include "tile.h"


class GameEngine
{
	
public:
	GameEngine(Ogre::SceneManager*, Gorilla::Screen *screen);
	virtual ~GameEngine();
	void tick();
	void init();
	void handleKeyEvent(OIS::KeyCode, bool);
	void handleMouseEvent(Ogre::Vector3 vec, bool, bool, int, int);
	void updateCamera(Ogre::Camera *);
	bool getInit() { return mInitialized; }
	Ogre::SceneManager* getSceneMgr() { return mSceneMgr; }
	void setHUDSizeFactor(double factor);
	void addBillboardItemToWorld(BillboardItem &item, Ogre::String id);
	Tile* getCellState(int x, int y, WrapMode mode = USEOFFMAP, CellState offmap = EMPTY);
protected:
	Gorilla::Screen *mScreen;
	Gorilla::Layer *mLayer;
	Ogre::SceneManager *mSceneMgr; 
	Ogre::Overlay *mInventoryOverlay;
	Ogre::OverlayManager *mOverlayMgr;
	Ogre::ManualObject *mManObj;
	int mXSize, mYSize; // Board-size
	int mLastX, mLastY; // Used for storing the last ghost-piece location and position while dragging.
	Ogre::Vector3 mLookatPos;
	std::vector<Ogre::SceneNode *> mPiecesNodes;
	std::vector<std::vector<Tile *>> mTiles;
	bool mTickNext, mInitialized, mDirection;
	double mHUDSizeFactor;
	TileSetManager mTileSetMgr;
	float mTransparancy;
	
	Ogre::Vector3 getCamOffset();
	void updateHUD();
	void resizeHUD();
	void updateManualObject();
	void updatePieces();
	void createBillboardScreen();
	void removeBillboardScreen();
	void updateDataStructures();
	void placeGhostPiece(int x, int y);
	void useTexCoord(SpecialEffect dir, int c);
};

#endif // GAMEENGINE_H

