#ifndef CTANK_CLOCKOBJ_H
#define CTANK_CLOCKOBJ_H

#include "nullity/entity.h"
#include "nullity/object.h"

namespace ctank {

	BEGIN_ENTITY_CLASS(ClockObject)
	nullity::TimeStep		GetTime(Data* Data);
	void					SetTime(Data* Data, nullity::TimeStep Time);
	END_ENTITY_CLASS
	
	/// Creates a clock object.
	IClockObject*	CreateClockObject();
}

#endif