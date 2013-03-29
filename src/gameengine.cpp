#include "gameengine.h"
#include "gamedefines.h"
#include "tile.h"


GameEngine::GameEngine(Ogre::SceneManager *manager, Gorilla::Screen *screen)
{
	mScreen = screen;
	mInitialized = false;
	mSceneMgr = manager;
	mHUDSizeFactor = 1.1;
	mOverlayMgr = Ogre::OverlayManager::getSingletonPtr();
	
	mManObj = manager->createManualObject();
	Ogre::SceneNode *node = manager->createSceneNode("board");
	node->attachObject(mManObj);
	node->setPosition(0,0,0);
	manager->getRootSceneNode()->addChild(node);
	mXSize = mYSize = 10;
	mLookatPos = Ogre::Vector3(mXSize*TILESIZE/2, mYSize*TILESIZE/2, 0);
	updateManualObject();
	updateDataStructures();
	mTiles[3][3]->setCellState(ALIVE);
	mTiles[4][3]->setCellState(ALIVE);
	mTiles[5][3]->setCellState(ALIVE);
	updatePieces();
	tempCount = 0;
}

void GameEngine::setHUDSizeFactor(double factor) {
	mHUDSizeFactor = std::max(0.1, factor);
	resizeHUD();
}

GameEngine::~GameEngine()
{
}

void GameEngine::init()
{
	
	Ogre::ResourceManager::ResourceMapIterator font = Ogre::FontManager::getSingleton().getResourceIterator();
	
	while( font.hasMoreElements() )
	{
		font.getNext()->load();
	}
	mInitialized = true;
	
}

void GameEngine::resizeHUD()
{
	updateHUD();
}

void GameEngine::updateHUD() 
{
}

void GameEngine::addBillboardItemToWorld(BillboardItem &item, Ogre::String id)
{
	Ogre::BillboardSet *set = mSceneMgr->createBillboardSet();
	set->setDefaultDimensions(WORLDSCALE, WORLDSCALE);
	set->setMaterialName(item.getResID());
	set->setBounds(Ogre::AxisAlignedBox::BOX_INFINITE, 0);
	item.setBillboard(set->createBillboard(0, 0, 0));
	item.getSceneName() = id;
	Ogre::SceneNode *node;
	node = mSceneMgr->getRootSceneNode()->createChildSceneNode(id);
	node->attachObject(set);
	node->setPosition(0, 0, 4);
}

CellState GameEngine::getCellState(int x, int y, WrapMode mode, CellState offmap)
{
	if (x < 0 || x >= mXSize || y < 0 || y >= mYSize) {
		return offmap;
	}
	return mTiles[x][y]->getCellState();
}


void GameEngine::tick()
{
	tempCount++;
	if (tempCount > 60) {
		tempCount-=60;
	} else {
		return;
	}
	updateHUD();
	for (int x = 0; x < mXSize; x++) {
		for (int y = 0; y < mYSize; y++) {
			mTiles[x][y]->calcAliveState();
		}
	}
	for (int x = 0; x < mXSize; x++) {
		for (int y = 0; y < mYSize; y++) {
			mTiles[x][y]->assignStoredState();
		}
	}
	updatePieces();
}


void GameEngine::setKeyState(OIS::KeyCode key, bool pressed) 
{
}

void GameEngine::createBillboardScreen()
{
	/*Ogre::OverlayContainer *bg = static_cast<Ogre::OverlayContainer *>(
	 *		mOverlayMgr->createOverlayElement("Panel", "itembg"));
	 *	bg->setMetricsMode(Ogre::GMM_RELATIVE);
	 *	bg->setDimensions(1-ICONRELDIST*2, 1-ICONRELDIST*2);
	 *	bg->setPosition(ICONRELDIST, ICONRELDIST);
	 *	bg->setMaterialName(materialType);
	 *	Ogre::TextAreaOverlayElement *listType = static_cast<Ogre::TextAreaOverlayElement*>
	 *	(mOverlayMgr->createOverlayElement("TextArea", scrType));
	 *	listType->setMetricsMode(Ogre::GMM_RELATIVE);
	 *	listType->setPosition(ICONRELSIZE - ICONRELDIST, ICONRELSIZE);
	 *	listType->setDimensions(1,1);
	 *	listType->setCharHeight(ICONRELSIZE);
	 *	listType->setColour(Ogre::ColourValue(0.6, 0.6, 0.6));
	 *	listType->setCaption(scrType);
	 *	listType->setFontName("bluecond");
	 *	bg->addChild(listType);
	 *	mInventoryOverlay->add2D(bg);
	 *	for (uint i = 0; i < billboards->getNumElements(); i++) {
	 *		Ogre::String resID = billboards->getResIDForEl(i);
	 *		Ogre::String name = billboards->getNameForEl(i);
	 *		Ogre::String sceneName = billboards->getSceneNameForEl(i);
	 *		Ogre::String tmp = "img" + sceneName;
	 *		Ogre::OverlayContainer *element = static_cast<Ogre::OverlayContainer *>
	 *		(mOverlayMgr->createOverlayElement("Panel", tmp));
	 *		element->setMetricsMode(Ogre::GMM_RELATIVE);
	 *		element->setMaterialName(resID);
	 *		element->setPosition(ICONRELSIZE,(ICONRELSIZE + ICONRELDIST)*(i+2));
	 *		element->setDimensions(ICONRELSIZE, ICONRELSIZE);
	 *		Ogre::TextAreaOverlayElement *textarea = static_cast<Ogre::TextAreaOverlayElement*>
	 *		(mOverlayMgr->createOverlayElement("TextArea", sceneName));
	 *		textarea->setMetricsMode(Ogre::GMM_RELATIVE);
	 *		textarea->setFontName("bluecond");
	 *		textarea->setCharHeight(ICONRELSIZE);
	 *		textarea->setDimensions(1,1);
	 *		textarea->setColour(Ogre::ColourValue(0.6, 0.6, 0.6));
	 *		textarea->setCaption(name);
	 *		textarea->setPosition(ICONRELSIZE + ICONRELDIST, 0.0);
	 *		element->addChild(textarea);
	 *		mInventoryOverlay->add2D(element);
	 }
	 mInventoryOverlay->show();*/
	
}

void GameEngine::removeBillboardScreen()
{
	Ogre::Overlay::Overlay2DElementsIterator itr = mInventoryOverlay->get2DElementsIterator();
	while (itr.hasMoreElements()) {
		Ogre::OverlayContainer *tmp = itr.getNext();
		auto children = tmp->getChildIterator();
		while (children.hasMoreElements()) {
			auto chld = children.getNext();
			mOverlayMgr->destroyOverlayElement(chld->getName());
		}
		mInventoryOverlay->remove2D(tmp);
		mOverlayMgr->destroyOverlayElement(tmp);
	}
	mInventoryOverlay->clear();
	mInventoryOverlay->hide();
	
}



void GameEngine::updateCamera(Ogre::Camera *camera)
{
	camera->setPosition(mLookatPos + Ogre::Vector3(0, -80, 80));
	camera->lookAt(mLookatPos);
	camera->setNearClipDistance(1);
	camera->setFarClipDistance(500);
}

void GameEngine::updateManualObject()
{
	mManObj->clear();
	mManObj->estimateIndexCount(mXSize*mYSize*6);
	mManObj->estimateVertexCount(mXSize*mYSize*4);
	mManObj->begin("defaulttile");
	uint32_t count = 0;
	for (int x = 0; x < mXSize; x++) {
		for (int y = 0; y < mYSize; y++) {
			count+=4;
			int xt = x * TILESIZE;
			int yt = y * TILESIZE;
			mManObj->textureCoord(0,0,0);
			mManObj->position(xt, yt, 0);
			mManObj->textureCoord(1,0,0);
			mManObj->position(xt+TILESIZE, yt, 0);
			mManObj->textureCoord(1,1,0);
			mManObj->position(xt+TILESIZE, yt+TILESIZE, 0);
			mManObj->textureCoord(0,1,0);
			mManObj->position(xt, yt+TILESIZE, 0);
			mManObj->quad(count-4, count-3, count-2, count-1);
		}
	}
	mManObj->end();
}

void GameEngine::updateDataStructures()
{
	for (std::vector<std::vector<Tile *>>::iterator itr = mTiles.begin(); itr != mTiles.end(); itr++) {
		for (uint i = 0; i < (*itr).size(); i ++) {
			delete (*itr)[i];
		}
	}
	mTiles.clear();
	for (int x = 0; x < mXSize; x++) {
		mTiles.push_back(std::vector<Tile *>());
		for (int y = 0; y < mYSize; y++) {
			mTiles[x].push_back(new Tile(this, x, y));
		}
	}
}

void GameEngine::updatePieces()
{
	for (std::vector<Ogre::SceneNode *>::iterator itr = mPiecesNodes.begin(); itr != mPiecesNodes.end(); itr++) {
		mSceneMgr->destroySceneNode((*itr));
	}
	mPiecesNodes.clear();
	for (int x = 0; x < mXSize; x++) {
		for (int y = 0; y < mYSize; y++) {
			if (mTiles[x][y]->getCellState() == EMPTY) {
				continue;
			}
			Ogre::Entity *ent = mSceneMgr->createEntity("blob.mesh");
			Ogre::SceneNode *node = mSceneMgr->createSceneNode();
			node->attachObject(ent);
			node->setPosition(x*TILESIZE, y*TILESIZE, 0);
			mPiecesNodes.push_back(node);
			mSceneMgr->getRootSceneNode()->addChild(node);
		}
	}
}
