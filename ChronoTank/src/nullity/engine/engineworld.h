#ifndef NULLITY_ENGINE_WORLD_H
#define NULLITY_ENGINE_WORLD_H

#include <vector>
#include <map>

#include "enginereality.h"
#include "engineentity.h"
#include "engineframe.h"

#include "../eventlog.h"
#include "../world.h"

namespace nullity {
	struct MessageEvent;
	struct MessageEx;
	class World;

	/// An event that indicates a messages is recieved.
	struct MessageEvent : public EventLog::Event {
		MessageEx*		MessageData;

		const static int		Flag = 0x00001000;

		void Remove();
	};

	/// An extended message containing information about its
	/// sender, receiver and time.
	struct MessageEx {
		Entity*				From;
		TimeStep			Source;
		Entity*				To;
		TimeStep			Destination;

		/// The actual message data.
		IMessage*			Message;

		/// The corresponding received event for this message
		/// definition.
		MessageEvent*		Event;
	};

	/// An implemented interface to a world.
	class World : public IWorld {
	public:
		void		Init(IObject* Root);
		void		Destroy();

		IFrame*		GetOriginFrame();
		IReality*	GetRootReality();

		/// Creates a new frame for the world.
		Frame*		CreateFrame();

		/// Creates a new entity for use in the world.
		Entity*		CreateEntity();

	private:
		Reality*					_rootreality;
		std::vector<Entity>			_entities;
		std::vector<Frame>			_frames;
	};


}

#endif