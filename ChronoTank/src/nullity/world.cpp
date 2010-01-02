/************************************************/
/*	World Implementation     					*/
/************************************************/
#include <vector>
#include <map>
#include <limits>

#include "engine/engineworld.h"

using namespace nullity;

/****************************************/
/*	World from engine					*/
/****************************************/
//--
StackPtr<IWorld> nullity::CreateWorld() {
	return new World();
}