#ifndef SOUNDSYSTEM_H
#define SOUNDSYSTEM_H
#include<AL/al.h>
#include<AL/alc.h>
#include<AL/alut.h>
#include<iostream>
#include<OgreVector3.h>
#include "gameevent.h"
#define NUM_BUFFERS 64
#define NUM_SOURCES 64

class SoundSystem
{
	
public:
	SoundSystem();
	virtual ~SoundSystem();
	void setCamPos(Ogre::Vector3 pos);
	void playSound(GameEvent event);
	
private:
	ALuint mBuffers[NUM_BUFFERS], mSources[NUM_SOURCES];
	Ogre::Vector3 mPos;
	int mCurSource;
};

#endif // SOUNDSYSTEM_H
