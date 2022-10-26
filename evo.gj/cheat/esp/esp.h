#pragma once 
#include "../../util/util.h"
#include "../sdk/sdk.h"
#include "../aimbot/aimbot.h"
class Esp {
public:
	static void ActorLoop();
};
inline bool bCornerBox = true;

namespace Debug {
	static bool PrintPointers = false;
	static bool PrintLocations = false;
}