#ifndef PRESET_DEAD_REC_H_
#define PRESET_DEAD_REC_H_


#include <vector>
#include <string>
#include <map>

//include walk commands
#include "PresetWalkParam.h"
#include "../Utility/TMat.h"

class PresetDeadRec
{
public:
	static std::vector<Point3D> deadRecData;
	PresetDeadRec();
	static int ResolveWalkName(const char* walkname);

private:
	static void RegisterWalkName(const char* walkname);
	static std::map<std::string,int> walkNameMap;
	static bool isInitialized;
};

#endif
