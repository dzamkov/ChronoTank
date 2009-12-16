#ifndef CTANK_TIMEMANIPULATOR_H
#define CTANK_TIMEMANIPULATOR_H

#include "object.h"
#include "world.h"

namespace ctank {
	
	/// An object that distorts the passage of time in the
	/// area surronding it.
	class ITimeManipulator : public IObject {
	public:

		/// Gets the amount of change of the rate of time at the
		/// specified position. A value of 1.0 indicates that time
		/// goes at a normal speed at the position, a value of 2.0
		/// indicates time is going twice its normal rate at the position.
		virtual TimeStep	GetTimeRateAtPosition(Vector Position) = 0;

		int					GetSpecialFlag();

		/// Special flag for time manipulators.
		static const int SpecialFlag	= 0x1337BEEF;
		
	};

	/// A time manipulator that changes the rate of time near it.
	class IDistortTimeManipulator : public ITimeManipulator {
	public:
		IDistortTimeManipulator();

		void			Destroy();

		Vector			GetPosition();
		Vector			GetRotation();

		/// Sets the center of the time manipulator.
		void			SetPosition(Vector Position);

		/// Sets the amount that time rate is changed by
		/// at the center of the manipulator.
		void			SetTimeDistortion(TimeStep Amount);

		/// Gets the amount time rate is changed by at
		/// the center of the manipulator.
		TimeStep		GetTimeDistortion();

		/// Sets the maximum distance at which time is affected.
		void			SetRadius(float Length);

		/// Gets the maximum distance at which time is affected.
		float			GetRadius();

	private:
		Vector		_position;
		float		_radius;
		TimeStep	_timedistortion;
	};

	/// Creates a basic distort time manipulator that interpolates
	/// time linearly around it.
	IDistortTimeManipulator*	CreateLinearDistortTimeManipulator();

	/// Creates a distort time manipulator that treats objects as
	/// either in it or not with no interpolation.
	IDistortTimeManipulator*	CreateSharpDistortTimeManipulator();

}

#endif