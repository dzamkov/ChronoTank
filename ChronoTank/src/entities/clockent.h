#ifndef CTANK_ENTITIES_CLOCK_H
#define CTANK_ENTITIES_CLOCK_H

#include "../nullity/entity.h"

namespace ctank {
	using namespace nullity;

	/// A debug entity for the purpose of showing time.
	class ClockEntity : public Entity {
	public:
		ClockEntity();

		void			Update(TimeStep Time);
		void			Clone(Ptr<Entity> To);
		Ptr<IVisual>	CreateVisual(VisualParameters Params);
		EntityClass*	GetClass();

		/// The class for clock entities.
		static EntityClass*		Class;

		/// Gets the time the clock displays.
		TimeStep				GetTime();

		/// Sets the time the clock should display and continue
		/// the normal behavior of the clock.
		void					SetTime(TimeStep Time);

	private:
		TimeStep	_time;
	};
}

#endif