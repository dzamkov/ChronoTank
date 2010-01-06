#ifndef CTANK_ENTITIES_UNIT_H
#define CTANK_ENTITIES_UNIT_H

#include "../nullity/entity.h"

namespace ctank {
	using namespace nullity;

	/// An individual player-controllable entity that affects
	/// other units and entities.
	class IUnit : public Entity {
	public:

		/// Turns the unit the specified velocity between -1.0f and 1.0f.
		virtual void		Turn(float Amount) = 0;

		/// Moves the unit foward by the specified velocity between -1.0f and 1.0f.
		virtual void		Move(float Amount) = 0;

	};

	/// Class for player tanks, a simple unit for testing purposes.
	EntityClass*		PlayerTankClass;

}

#endif