#ifndef CTANK_EVENTLOG_H
#define CTANK_EVENTLOG_H

#include <vector>

#include "object.h"

namespace ctank {

	/// Event log that can be used by worlds to store
	/// the time certain events can happen. This is
	/// an optional implemented helper class and not an
	/// interface.
	class EventLog {
	public:
		EventLog();
		~EventLog();

		/// Recorded event at a certain time.
		class Event {
		public:
			TimeStep		Time;
			int				Flags;

			/// Removes the event.
			virtual void	Remove();
		};

		/// Event that describes an objects state at
		/// a certain time.
		class ObjectStateEvent : public Event {
		public:
			IEntity*		Entity;
			IObject*		State;

			/// Flag used on object state events.
			static const int	Flag	= 0x00000001;

			/// Flag used on object state events where the
			/// state is automatically deleted when the event
			/// is removed.
			static const int	AutoDeleteFlag	= 0x00000002 | Flag;

			virtual void	Remove();
		};

		/// Event that is used when the object is removed from
		/// the world.
		class ObjectRemovalEvent : public Event {
		public:
			IEntity*		Entity;

			/// Flag used on object removal events.
			static const int	Flag	= 0x00000010;

			virtual void Remove();
		};

		/// Removes the events between the start and end times(inclusive) where
		/// the event has all the specified flags.
		void			RemoveEvents(TimeStep Start, TimeStep End, int Flags);

		/// Adds an event to the log.
		void			AddEvent(Event* Event);

		/// Removes an event from the log.
		void			RemoveEvent(Event* Event);

		/// Gets the events at the specified time range that have the
		/// specified flags.
		std::vector<const Event*>		GetEvents(TimeStep Start, TimeStep End, int Flags);

		/// Gets the state of the object at the specified time as described by
		/// object state events. The state is used in records, so it must be
		/// cloned before being updated/interacted with.
		IObject*		GetObjectStateAtTime(IEntity* Entity, TimeStep Time);

		/// Gets the state of all objects at the time.
		std::vector<IObject*>		GetObjectStatesAtTime(TimeStep Time);

		/// Adds an object state event to the log.
		void			AddObjectStateEvent(TimeStep Time, IObject* Object, bool Clone, bool AutoDelete);

	private:
		std::vector<Event*>		_events;
	};
}

#endif