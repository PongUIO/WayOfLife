#ifndef GAMEENGINE_H
#define GAMEENGINE_H

class Creature;
class GameEngine;
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
#include "billboarditem.h"
#include "../gorilla/Gorilla.h"
#include "tile.h"
#include "eventmanager.h"
#include "soundsystem.h"
#include "filemanager.h"


class GameEngine
{
	
public:
	GameEngine(Ogre::SceneManager*, Gorilla::Screen *screen);
	virtual ~GameEngine(void);
	void tick(Ogre::Real dt);
	void init(void);
	void handleKeyEvent(OIS::KeyCode, bool);
	void handleMouseEvent(Ogre::Vector3 vec, bool, bool, int, int);
	void updateCamera(Ogre::Camera *);
	bool getInit(void) { return mInitialized; }
	Ogre::SceneManager* getSceneMgr(void) { return mSceneMgr; }
	void setHUDSizeFactor(double factor);
	void addBillboardItemToWorld(BillboardItem &item, Ogre::String id);
	Tile* getTile(int x, int y, WrapMode mode = USEOFFMAP, State offmap = EMPTY);
	EventManager &getEventMan(void) { return mEventMan; }
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
	std::vector<std::vector<Tile>> mTiles;
	bool mTickNext, mInitialized, mDirection;
	double mHUDSizeFactor;
	float mTransparancy;
	SoundSystem mSoundSystem;
	EventManager mEventMan;
	FileManager mFileManager;
	
	Ogre::Vector3 getCamOffset(void);
	void updateHUD(void);
	void resizeHUD(void);
	void updateManualObject(void);
	void updatePieces(void);
	void createBillboardScreen(void);
	void removeBillboardScreen(void);
	void placeGhostPiece(int x, int y);
	void useTexCoord(SpecialEffect dir, int c);
	void setLevel(int level);
	void setDimensions(int x, int y, bool fullLoad = false);
	MapInfo createMapInfo(void);
	
};

#endif // GAMEENGINE_H

