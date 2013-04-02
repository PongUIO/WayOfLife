#ifndef FILELOADER_H
#define FILELOADER_H
#include<OgreString.h>
#include<OgreFileSystem.h>
#include "mapinfo.h"

class FileLoader
{
	
public:
	FileLoader();
	virtual ~FileLoader();
	MapInfo *getMap(int map);
	void loadAllMaps(std::string path);
private:
	std::string mPath;
	void loadMap(std::string file, Ogre::FileSystemArchive *archive);
	std::vector<MapInfo> mMaps;
};

#endif // FILELOADER_H
