#ifndef BILLBOARDITEM_H
#define BILLBOARDITEM_H

#include<OgreString.h>
#include<OgreBillboard.h>

class BillboardItem
{
public:
	const Ogre::String &getResID(void) const { return mResID; }
	Ogre::String &getSceneName(void) { return mSceneName; };
	Ogre::String &getName(void) { return mName; };
	void setBillboard(Ogre::Billboard *b) { mBillboard = b; }
	Ogre::Billboard *getBillboard(void) { return mBillboard; }
	Ogre::String mSceneName, mResID, mName;
	Ogre::Billboard *mBillboard;
};
#endif