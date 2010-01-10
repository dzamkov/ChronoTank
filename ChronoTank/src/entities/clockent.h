#ifndef CTANK_ENTITIES_CLOCK_H
#define CTANK_ENTITIES_CLOCK_H

#include "../nullity/entity.h"

namespace ctank {
	using namespace nullity;
	
	/// Interface to an entity that shows time.
	class IClockEntity : public Interface {
	public:
		DECLARE_INTERFACE_CLASS(IClockEntity)

		/// Gets the time displayed.
		virtual TimeStep		GetTime() = 0;

		/// Sets the time the clock should show.
		virtual void			SetTime(TimeStep Time) = 0;
	};

	/// Implemented clock entity that displays time in MMM:SS:III format
	/// directly on the screen at its location.
	class ClockEntity : public Interface, 
		public IClockEntity, 
		public IVisualEntity, 
		public IDynamicEntity, 
		public IEntityInterface {
	public:
		DECLARE_INTERFACE_CLASS(ClockEntity)

		void			Update(TimeStep Time);
		Ptr<Entity>		Clone();
		Ptr<IVisual>	CreateVisual(VisualParameters Params);

		TimeStep		GetTime();
		void			SetTime(TimeStep Time);

	private:
		TimeStep	_time;
	};
}

#endif