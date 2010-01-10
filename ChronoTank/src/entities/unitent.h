#ifndef CTANK_ENTITIES_UNIT_H
#define CTANK_ENTITIES_UNIT_H

#include "../nullity/entity.h"

namespace ctank {
	using namespace nullity;

	/// An individual player-controllable entity that affects
	/// other units and entities.
	class IUnit : public Interface {
	public:
		DECLARE_INTERFACE_CLASS(IUnit)

		/// Turns the unit the specified velocity between -1.0f and 1.0f.
		virtual void		Turn(float Amount) = 0;

		/// Moves the unit foward by the specified velocity between -1.0f and 1.0f.
		virtual void		Move(float Amount) = 0;
	};

	/// Creates a simple player-controllable tank.
	Ptr<Entity>		CreatePlayerTank();

}

#endif