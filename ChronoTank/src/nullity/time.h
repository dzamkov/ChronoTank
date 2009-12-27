#ifndef NULLITY_TIME_H
#define NULLITY_TIME_H

namespace nullity {

	/// Defines a timestep.
	typedef	float		TimeStep;

	/// Defines an absolute time.
	typedef TimeStep	Time;

	/// Gets the time the specified amount after the specified time.
	Time		After(Time Time, TimeStep Amount);

}

#endif