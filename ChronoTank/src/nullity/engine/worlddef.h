#ifndef NULLITY_ENGINE_WORLDDEF_H
#define NULLITY_ENGINE_WORLDDEF_H

#include <vector>
#include <map>

#include "entitydef.h"
#include "framedef.h"

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

		IFrame*		CreateOriginFrame();

		/// Creates a new frame for the world.
		Frame*		CreateFrame();

		/// Creates a new entity for use in the world.
		Entity*		CreateEntity();

		/// Creates a new message for use in the world. The
		/// message is not sent until SendMessage is called.
		MessageEx*	CreateMessage();

		/// Sends the specified message. A MessageEx can only be
		/// sent if it was never sent before or if it was cancelled.
		/// The message data can be reused.
		void		SendMessage(MessageEx* Message);

		/// Cancels a message so it will no longer be received.
		void		CancelMessage(MessageEx* Message);

		/// Loads object data for the specified time into the frame.
		void		LoadIntoFrame(TimeStep Time, Frame* Frame);

		/// Records the state of the entity that belongs to the supplied
		/// object at the specified time.
		void		RecordState(TimeStep Time, IObject* State);

		/// Gets a map of entities and the messages they received in a certain time
		/// period.
		std::map<Entity*, std::vector<MessageEx*>>		GetReceivedMessages(TimeStep Start, TimeStep End);

	private:

		std::vector<MessageEx>		_messages;
		std::vector<Entity>			_entities;
		std::vector<Frame>			_frames;
		EventLog					_log;

		std::map<IMessage*, std::vector<MessageEx*>>	_messageusages;
	};


}

#endif