#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include<OgreString.h>
#include<OgreFileSystem.h>
#include "mapinfo.h"

class FileManager
{
	
public:
	FileManager();
	virtual ~FileManager();
	MapInfo *getMap(int map) { return &mMaps[map]; }
	void addMap(MapInfo m) { mMaps.push_back(m); }
	void loadAllMaps(std::string path);
private:
	std::string mPath;
	void loadMap(std::string file, Ogre::FileSystemArchive *archive);
	std::vector<MapInfo> mMaps;
};

#endif // FILEMANAGER_H
