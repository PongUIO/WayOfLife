#include "gameengine.h"
#include "gamedefines.h"
#include "tile.h"


GameEngine::GameEngine(Ogre::SceneManager *manager, Gorilla::Screen *screen) 
			: mSoundSystem(), mEventMan(&mSoundSystem), mFileManager()
{
	mScreen = screen;
	mSceneMgr = manager;
	mFileManager.loadAllMaps("../WayOfLifeAssets/levels/");
	mInitialized = mDirection = mTickNext = false;
	mHUDSizeFactor = 1.1;
	mLastX = mLastY = -1;
	mTransparancy = 0.5f;
	mXSize = mYSize = 0;
	mOverlayMgr = Ogre::OverlayManager::getSingletonPtr();
	mLookatPos = Ogre::Vector3(mXSize*TILESIZE/2, mYSize*TILESIZE/2, 0);
	mSoundSystem.setCamPos(mLookatPos + getCamOffset());
	mManObj = manager->createManualObject();
	mEditMode = true;
	
	Ogre::SceneNode *node = manager->createSceneNode("board");
	node->attachObject(mManObj);
	node->setPosition(0,0,0);
	manager->getRootSceneNode()->addChild(node);
	setLevel(0);
}

void GameEngine::setLevel(int level)
{

	MapInfo *info = mFileManager.getMap(level);
	setDimensions(info->mX, info->mY, true);
	for (uint i = 0; i < (uint) mXSize; i++) 
	{
		for (uint j = 0; j < (uint) mYSize; j++) {
			size_t ind = i + j * mXSize;
			if (info->mMap.size() > ind && i < mTiles.size() && j < mTiles[i].size()) {
				mTiles[i][j].setSpecialEffect(info->mMap[ind]);
			}
			if (info->mCells.size() > ind && i < mTiles.size() && j < mTiles[i].size()) {
				mTiles[i][j].setState(info->mCells[ind]);
				mTiles[i][j].setStoreState(info->mCells[ind]);
			}
		}
	}
	updateManualObject();
	updatePieces();
}
void GameEngine::setDimensions(int xN, int yN, bool fullLoad)
{
	xN = std::max(std::min(MAXBOARDSIZE, xN), 1);
	yN = std::max(std::min(MAXBOARDSIZE, yN), 1);
	while ((int)mTiles.size() < xN) {
		mTiles.push_back(std::vector<Tile>());
	}
	while ((int)mTiles.size() > xN) {
		mTiles.pop_back();
	}
	for (int x = 0; x < xN; x++) {
		for (int y = 0; y < yN; y++) {
			if (y >= (int) mTiles[x].size()) {
				mTiles[x].push_back(Tile(this, x, y));
				if (!fullLoad) {
					mTiles[x][y].setLoaded(10);
				}
			}
			if (fullLoad) {
				mTiles[x][y].setLoaded(yN - x +  xN - y + 5);
			}
		}
		while ((int) mTiles[x].size() > yN) {
			mTiles[x].pop_back();
		}
	}
	mXSize = xN;
	mYSize = yN;
}

MapInfo GameEngine::createMapInfo(void)
{
	MapInfo info;
	info.mX = mXSize;
	info.mY = mYSize;
	for (int y = 0; y < mYSize; y++) {
		for (int x = 0; x < mXSize; x++) {
			info.mMap.push_back(mTiles[x][y].getSpecialEffect());
			info.mCells.push_back(mTiles[x][y].getState());
		}
	}
	return info;
}

void GameEngine::setHUDSizeFactor(double factor) {
	mHUDSizeFactor = std::max(0.1, factor);
	resizeHUD();
}

GameEngine::~GameEngine(void)
{
}

void GameEngine::init(void)
{
	
	Ogre::ResourceManager::ResourceMapIterator font = Ogre::FontManager::getSingleton().getResourceIterator();
	
	while( font.hasMoreElements() )
	{
		font.getNext()->load();
	}
	mInitialized = true;
	
}

void GameEngine::resizeHUD(void)
{
	updateHUD();
}

void GameEngine::updateHUD(void) 
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
	node->setPosition(0, 0, 0);
}

Tile *GameEngine::getTile(int x, int y, WrapMode mode, State offmap)
{
	if (x < 0 || x >= mXSize || y < 0 || y >= mYSize) {
		return NULL;
	}
	return &mTiles[x][y];
}


void GameEngine::tick(Ogre::Real dt)
{
	mEventMan.clearAll();
	updateHUD();
	if (!getMapLoading()) {
		for (int x = 0; x < mXSize; x++) {
			for (int y = 0; y < mYSize; y++) {
				mTiles[x][y].updateLoaded(dt*10);
			}
		}
		updateManualObject();
		updatePieces();
	}
	if (!mTickNext) {
		return;
	}
	mTickNext = false;
	for (int x = 0; x < mXSize; x++) {
		for (int y = 0; y < mYSize; y++) {
			if (mTiles[x][y].getInheritedSpecialEffect() == NONE || mTiles[x][y].getState() == EMPTY) {
				mTiles[x][y].calcAliveState();
				mTiles[x][y].setDone(true);
			}
		}
	}
	for (int x = 0; x < mXSize; x++) {
		for (int y = 0; y < mYSize; y++) {
			if (mTiles[x][y].getDone()) {
				mTiles[x][y].assignStoredState();
			}
		}
	}
	for (int x = 0; x < mXSize; x++) {
		for (int y = 0; y < mYSize; y++) {
			if (!mTiles[x][y].getDone()) {
				mTiles[x][y].setStoreState(EMPTY);
			}
		}
	}
	for (int x = 0; x < mXSize; x++) {
		for (int y = 0; y < mYSize; y++) {
			if (!mTiles[x][y].getDone()) {
				mTiles[x][y].calcAliveState();
			}
		}
	}
	for (int x = 0; x < mXSize; x++) {
		for (int y = 0; y < mYSize; y++) {
			mTiles[x][y].assignStoredState();
			mTiles[x][y].assignStoredEffect();
			mTiles[x][y].setDone(false);
		}
	}
	updatePieces();
	std::vector<GameEvent> *event = mEventMan.getEvents();
	for (uint i = 0; i < event->size(); i++) {
		mSoundSystem.playSound(event->at(i));
	}
}


void GameEngine::handleKeyEvent(OIS::KeyCode key, bool pressed) 
{
	bool resize = false;
	if (key == OIS::KC_SPACE && pressed) {
		if (!getMapLoading()) {
			return;
		}
		mTickNext = true;
	} else if (key == OIS::KC_A && pressed) {
		setDimensions(mXSize+1, mYSize);
		resize = true;
	} else if (key == OIS::KC_Z && pressed) {
		setDimensions(mXSize-1, mYSize);
		resize = true;
	} else if (key == OIS::KC_S && pressed) {
		setDimensions(mXSize, mYSize+1);
		resize = true;
	} else if (key == OIS::KC_X && pressed) {
		setDimensions(mXSize, mYSize-1);
		resize = true;
	}
	
	if (resize) {
		updateManualObject();
		updatePieces();
	}
}

void GameEngine::handleMouseEvent(Ogre::Vector3 vec, bool pressed, bool right, int x, int y)
{
	if (right) {
		if (mLastX != -1) {
			updatePieces();
		}
		if (!pressed) {
			mLookatPos.x += (mLastX-x)*0.2;
			mLookatPos.y += (y-mLastY)*0.2;
		}
		mLastX = x;
		mLastY = y;
	} else {
		if (!getMapLoading()) {
			return;
		}
		Ogre::Vector3 dist =  mLookatPos + getCamOffset();
		if (std::abs(vec.z) < 0.001) {
			return;
		}
		double zDiff = dist.z/vec.z;
		Ogre::Vector3 outPos = mLookatPos + getCamOffset() - zDiff*vec;
		int x = outPos.x/TILESIZE;
		int y = outPos.y/TILESIZE;
		if (x < 0 || x >= mXSize || y < 0 || y >= mYSize || (mTiles[x][y].getState() != EMPTY && !pressed)) {
			if (mLastX != -1) {
				updatePieces();
			}
			return;
		}
		if (pressed) {
			if (mTiles[x][y].getSpecialEffect() == AIR) {
				return;
			}
			State state = mTiles[x][y].getState();
			mTiles[x][y].setInheritedSpecialEffect(NONE);
			if (state == EMPTY) {
				mTiles[x][y].setState(ALIVE);
				updatePieces();
			} else {
				mTiles[x][y].setState(EMPTY);
				placeGhostPiece(x, y);
			}
			if (mTiles[x][y].getSpecialEffect() != NONE && mTiles[x][y].getState() == ALIVE) {
				mSoundSystem.playSound(GameEvent(SPEEDUP, x, y));
			} else {
				mSoundSystem.playSound(GameEvent(CREATION, x, y));
			}
		} else {
			if ((mLastX != x || mLastY != y) && mTiles[x][y].getState() == EMPTY) {
				placeGhostPiece(x, y);
				mLastX = x;
				mLastX = y;
			}
		}
	}
}

void GameEngine::placeGhostPiece(int x, int y)
{
	updatePieces();
	if (!getMapLoading()) {
		return;
	}
	if (mTiles[x][y].getSpecialEffect() == AIR) {
		return;
	}
	Ogre::Entity *ent = mSceneMgr->createEntity("blob.mesh");
	Ogre::MaterialPtr mat;
	if (mTiles[x][y].getSpecialEffect() != NONE ) {
		mat = Ogre::MaterialManager::getSingletonPtr()->getByName("speedblob");
		mat = mat->clone("speedblobtrans");
	} else {
		mat = ent->getSubEntity(0)->getMaterial()->clone("blobtrans");
	}
	Ogre::Pass *p = mat->getTechnique(0)->getPass(0);
	p->setSceneBlending(Ogre::SceneBlendType::SBT_TRANSPARENT_ALPHA);
	p->setDepthWriteEnabled(false);
	Ogre::TextureUnitState *tus = p->createTextureUnitState();
	tus->setAlphaOperation(Ogre::LayerBlendOperationEx::LBX_SOURCE1,
			Ogre::LayerBlendSource::LBS_MANUAL,
			Ogre::LayerBlendSource::LBS_CURRENT,
			mTransparancy
	);
	ent->setMaterial(mat);
	Ogre::SceneNode *node = mSceneMgr->createSceneNode();
	node->attachObject(ent);
	node->setPosition(x*TILESIZE, y*TILESIZE, 0);
	mPiecesNodes.push_back(node);
	mSceneMgr->getRootSceneNode()->addChild(node);
}


void GameEngine::createBillboardScreen(void)
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

void GameEngine::removeBillboardScreen(void)
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
	mSoundSystem.setCamPos(mLookatPos + getCamOffset());
	camera->setPosition(mLookatPos + getCamOffset());
	camera->lookAt(mLookatPos);
	camera->setNearClipDistance(1);
	camera->setFarClipDistance(500);
}

Ogre::Vector3 GameEngine::getCamOffset(void)
{
	return Ogre::Vector3(0, -80, 80);
}

void GameEngine::updateManualObject(void)
{
	mManObj->clear();
	mManObj->estimateIndexCount(mXSize*mYSize*6);
	mManObj->estimateVertexCount(mXSize*mYSize*4);
	uint32_t count;
	for (int i = 0; i <= SpecialEffect::MOVLEFT; i++) {
		if (i == SpecialEffect::AIR) {
			continue;
		}
		if (i == 0) {
			count = 0;
			mManObj->begin("defaulttile");
		} else if (i == SpecialEffect::MOVUP) {
			count = 0;
			mManObj->end();
			mManObj->begin("speedtile");
		}
		for (int x = 0; x < mXSize; x++) {
			for (int y = 0; y < mYSize; y++) {
				if (mTiles[x][y].getSpecialEffect() != i) {
					continue;
				}
				count+=4;
				int xt = x * TILESIZE;
				int yt = y * TILESIZE;
				Ogre::Real z = mTiles[x][y].getLoaded();
				mManObj->position(xt, yt, z);
				useTexCoord((SpecialEffect)i, 0);
				mManObj->position(xt+TILESIZE, yt, z);
				useTexCoord((SpecialEffect)i, 1);
				mManObj->position(xt+TILESIZE, yt+TILESIZE, z);
				useTexCoord((SpecialEffect)i, 2);
				mManObj->position(xt, yt+TILESIZE, z);
				useTexCoord((SpecialEffect)i, 3);
				mManObj->quad(count-4, count-3, count-2, count-1);
			}
		}
	}
	mManObj->end();
}


void GameEngine::useTexCoord(SpecialEffect dir, int c)
{
	if (!(dir == NONE || dir == MOVUP)) {
		c += dir-SpecialEffect::MOVUP;
		c%=4;
	}
	switch(c) {
		case 0:
			mManObj->textureCoord(0,1,0);
			break;
		case 1:
			mManObj->textureCoord(1,1,0);
			break;
		case 2:
			mManObj->textureCoord(1,0,0);
			break;
		case 3:
			mManObj->textureCoord(0,0,0);
			break;
	}
	
}

void GameEngine::updatePieces(void)
{
	for (std::vector<Ogre::SceneNode *>::iterator itr = mPiecesNodes.begin(); itr != mPiecesNodes.end(); itr++) {
		mSceneMgr->destroySceneNode((*itr));
	}
	mPiecesNodes.clear();
	for (int x = 0; x < mXSize; x++) {
		for (int y = 0; y < mYSize; y++) {
			if (mTiles[x][y].getState() == EMPTY) {
				continue;
			}
			Ogre::Entity *ent = mSceneMgr->createEntity("blob.mesh");
			if (mTiles[x][y].getState() == SOLID) {
				ent->setMaterialName("solidblob");
			} else if (mTiles[x][y].getSpecialEffect() != NONE || mTiles[x][y].getInheritedSpecialEffect() != NONE) {
				ent->setMaterialName("speedblob");
			}
			Ogre::SceneNode *node = mSceneMgr->createSceneNode();
			node->attachObject(ent);
			Ogre::Real z = mTiles[x][y].getLoaded();
			node->setPosition(x*TILESIZE, y*TILESIZE, z);
			mPiecesNodes.push_back(node);
			mSceneMgr->getRootSceneNode()->addChild(node);
		}
	}
}
