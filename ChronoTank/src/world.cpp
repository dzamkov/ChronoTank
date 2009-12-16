/************************************************/
/*	World Implementation     					*/
/************************************************/
#include <vector>
#include <map>
#include <limits>

#include "engine/worlddef.h"

using namespace ctank;

/****************************************/
/*	World from engine					*/
/****************************************/
//--
IWorld* ctank::CreateWorld() {
	return new World();
}