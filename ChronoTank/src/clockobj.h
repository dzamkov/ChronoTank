#ifndef CTANK_CLOCKOBJ_H
#define CTANK_CLOCKOBJ_H

#include "object.h"

namespace ctank {

	/// An object that counts time.
	class IClockObject : public IObject {
	public:
		
		/// Gets the amount of seconds that passed for the clock.
		virtual TimeStep		GetTime() = 0;

		/// Sets the clocks time in seconds. A negative value will
		/// create a countdown clock.
		virtual void		SetTime(TimeStep Time) = 0;

		/// Sets the location of the clock.
		virtual void		SetPosition(Vector Position) = 0;
	};
	
	/// Creates a clock object.
	IClockObject*	CreateClockObject();
}

#endif