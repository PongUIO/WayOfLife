#ifndef MAPINFO_H
#define MAPINFO_H
#include<vector>
#include "gamedefines.h"

class MapInfo
{
	
public:
	MapInfo();
	virtual ~MapInfo();
	std::vector<SpecialEffect> mMap;
	int mX, mY;
};

#endif // MAPINFO_H
