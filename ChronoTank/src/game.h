#ifndef CTANK_GAME_H
#define CTANK_GAME_H

#include <irrlicht.h>

#include "nullity/object.h"

namespace ctank {
	
	/// Top-level interface for an application.
	class IGame : public irr::IEventReceiver {
	public:

		/// Called when the game is started/created.
		virtual void		Init(irr::IrrlichtDevice* Device) = 0;

		/// Called when the game must advance an amount of time.
		virtual void		Update(nullity::TimeStep Time) = 0;

		/// Called when the game must be rendered.
		virtual void		Render() = 0;

		/// Called when the game is ended.
		virtual void		Destroy() = 0;
	};

	IGame*		CreateGame();
}

#endif