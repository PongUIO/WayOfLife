#include "fileloader.h"
#include <OgreConfigFile.h>

FileLoader::FileLoader()
{
}

void FileLoader::loadMap(std::string file, Ogre::FileSystemArchive *archive)
{
	Ogre::DataStreamPtr ptr = archive->open(file);
	MapInfo info;
	Ogre::ConfigFile config;
	Ogre::String map = "", cells = "";
	config.load(ptr);
	info.mX = std::stoi(config.getSetting("xs", Ogre::StringUtil::BLANK, "-1"));
	info.mY = std::stoi(config.getSetting("ys", Ogre::StringUtil::BLANK, "-1"));
	map = config.getSetting("map");
	cells = config.getSetting("cells");
	for (size_t i = 0; i < map.size(); i++) {
		switch (map[i]) {
			case 'N':
				info.mMap.push_back(NONE);
				break;
			case 'A':
				info.mMap.push_back(AIR);
				break;
			case 'D':
				info.mMap.push_back(MOVDOWN);
				break;
			case 'U':
				info.mMap.push_back(MOVUP);
				break;
			case 'L':
				info.mMap.push_back(MOVLEFT);
				break;
			case 'R':
				info.mMap.push_back(MOVRIGHT);
				break;
			default:
				break;
		}
	}
	for (size_t i = 0; i < cells.size(); i++) {
		switch (cells[i]) {
			case 'E':
				info.mCells.push_back(EMPTY);
				break;
			case 'A':
				info.mCells.push_back(ALIVE);
				break;
			case 'S':
				info.mCells.push_back(SOLID);
				break;
			default:
				break;
		}
	}
	mMaps.push_back(info);
}

MapInfo *FileLoader::getMap(int map) {
	return &mMaps[map];
}

void FileLoader::loadAllMaps(std::string path)
{
	Ogre::FileSystemArchive archive(path, "");
	
	loadMap("testlevel.wol", &archive);
}

FileLoader::~FileLoader()
{

}

