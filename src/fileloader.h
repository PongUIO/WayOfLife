#ifndef FILELOADER_H
#define FILELOADER_H
#include<string.h>
#include<string>
#include "mapinfo.h"

class FileLoader
{
	
public:
	FileLoader(std::string path);
	virtual ~FileLoader();
	MapInfo loadMap(std::string file);
private:
	std::string mPath;
};

#endif // FILELOADER_H
