#ifndef BILLBOARDCOLLECTION_H
#define BILLBOARDCOLLECTION_H
#include <OgrePrerequisites.h>

class BillboardCollection
{

public:
	virtual uint getNumElements() = 0;
	virtual Ogre::String getResIDForEl(int i) = 0;
	virtual Ogre::String getNameForEl(int i) = 0;
	virtual Ogre::String getSceneNameForEl(int i) = 0;
};

#endif // BILLBOARDCOLLECTION_H
