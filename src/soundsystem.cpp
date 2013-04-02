#include "soundsystem.h"


SoundSystem::SoundSystem()
{
	alutInit(0, nullptr);
	mCurSource = 0;
	
	ALenum error = alGetError();
	
	std::vector<std::string> files;
	
	
	files.push_back("../WayOfLifeAssets/click.wav");
	files.push_back("../WayOfLifeAssets/swap.wav");
	files.push_back("../WayOfLifeAssets/swap.wav");
	files.push_back("../WayOfLifeAssets/swap.wav");
	files.push_back("../WayOfLifeAssets/speedup.wav");
	files.push_back("../WayOfLifeAssets/speedup.wav");
	for (uint i = 0; i < files.size(); i++) {
	
		mBuffers[i] = alutCreateBufferFromFile(files[i].data());
		if ((error = alGetError()) != AL_NO_ERROR) {
			std::cout << "alutCreateBufferFromFile:" << alGetString(alutGetError()) << std::endl;
		}
	}
	
	alGenSources(NUM_SOURCES, mSources);
	if ((error = alGetError()) != AL_NO_ERROR) {
		std::cout << "alGenSourcesi:" << alGetString(error) << std::endl;
		return;
	}
	
}

void SoundSystem::setCamPos(Ogre::Vector3 pos)
{
	ALfloat listenerPos[]={pos[0],pos[1],pos[2]};
	ALfloat listenerVel[]={0.0,0.0,0.0};
	ALfloat listenerOri[]={0.0,0.0,-1.0, 0.0,1.0,0.0};
	// Position ...
	alListenerfv(AL_POSITION,listenerPos);
	ALenum error;
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		return;
	}
	// Velocity ...
	alListenerfv(AL_VELOCITY,listenerVel);
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		return;
	}
	// Orientation ...
	alListenerfv(AL_ORIENTATION,listenerOri);
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		return;
	} 
	
}

void SoundSystem::playSound(GameEvent event) {
	alSourcei(mSources[mCurSource], AL_BUFFER, mBuffers[event.mType]);
	ALenum error;
	if ((error = alGetError()) != AL_NO_ERROR) {
		std::cout << "alSourcei:" << alGetString(error) << std::endl;
		return;
	}
	
	
	alSourcePlay(mSources[mCurSource]);
	mCurSource++;
	mCurSource %= NUM_SOURCES;
}


SoundSystem::~SoundSystem()
{
	
}

