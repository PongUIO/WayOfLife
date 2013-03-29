#ifndef __BaseApplication_h_
#define __BaseApplication_h_

#include<OgreBillboardSet.h>
#include<OgreCamera.h>
#include<OgreEntity.h>
#include<OgreLogManager.h>
#include<OgreRoot.h>
#include<OgreViewport.h>
#include<OgreSceneManager.h>
#include<OgreRenderWindow.h>
#include<OgreConfigFile.h>

#include<OISEvents.h>
#include<OISInputManager.h>
#include<OISKeyboard.h>
#include<OISMouse.h>

#include<SdkTrays.h>
#include<SdkCameraMan.h>

#include "gameengine.h"
#include "../gorilla/Gorilla.h"

class BaseApplication : public Ogre::FrameListener, public Ogre::WindowEventListener, public OIS::KeyListener, public OIS::MouseListener, OgreBites::SdkTrayListener
{
public:
	BaseApplication(void);
	virtual ~BaseApplication(void);
	
	virtual void go(void);
	
protected:
	virtual bool setup();
	virtual bool configure(void);
	virtual void chooseSceneManager(void);
	virtual void createCamera(void);
	virtual void createFrameListener(void);
	virtual void createScene(void);
	virtual void destroyScene(void);
	virtual void createViewports(void);
	virtual void setupResources(void);
	virtual void createResourceListener(void);
	virtual void loadResources(void);
	
	// Ogre::FrameListener
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	
	// OIS::KeyListener
	virtual bool keyPressed( const OIS::KeyEvent &arg );
	virtual bool keyReleased( const OIS::KeyEvent &arg );
	// OIS::MouseListener
	virtual bool mouseMoved( const OIS::MouseEvent &arg );
	virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	
	// Ogre::WindowEventListener
	//Adjust mouse clipping area
	virtual void windowResized(Ogre::RenderWindow* rw);
	//Unattach OIS before window shutdown (very important under Linux)
	virtual void windowClosed(Ogre::RenderWindow* rw);
	
	GameEngine *mEngine;
	
	Ogre::Root *mRoot;
	Ogre::Camera* mCamera;
	Ogre::SceneManager* mSceneMgr;
	Ogre::RenderWindow* mWindow;
	Ogre::String mResourcesCfg;
	Ogre::String mPluginsCfg;
	
	
	// OgreBites
	OgreBites::SdkTrayManager* mTrayMgr;
	OgreBites::ParamsPanel* mDetailsPanel;     // sample details panel
	bool mCursorWasVisible;                    // was cursor visible before dialog appeared
	bool mShutDown;
	void tick(void);
	
	//OIS Input devices
	OIS::InputManager* mInputManager;
	OIS::Mouse*    mMouse;
	OIS::Keyboard* mKeyboard;
	Ogre::Viewport* mViewport;
	
	Gorilla::Silverback *mGorilla;
	Gorilla::Screen *mScreen;
};

#endif
