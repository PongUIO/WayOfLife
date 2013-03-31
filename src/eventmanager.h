#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H
#include<vector>
#include "gameevent.h"
#include "soundsystem.h"

class EventManager
{
	
public:
	EventManager(SoundSystem *s);
	virtual ~EventManager();
	std::vector<GameEvent> *getEvents() { return &mEvents; }
	void addRequirement(GameEvent event) { mRequiredEvents.push_back(event); }
	void addFailure(GameEvent event) { mFailureEvents.push_back(event); }
	void addEvent(GameEvent event) { mEvents.push_back(event); }
	
	void clearAll() { mEvents.clear(); mRequiredEvents.clear(); mFailureEvents.clear(); }
	

private:
	std::vector<GameEvent> mEvents;
	std::vector<GameEvent> mRequiredEvents;
	std::vector<GameEvent> mFailureEvents;
	SoundSystem *mSoundSystem;
};

#endif // EVENTMANAGER_H
