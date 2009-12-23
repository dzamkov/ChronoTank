#ifndef NULLITY_ENGINE_REALITY_H
#define NULLITY_ENGINE_REALITY_H

#include <vector>
#include <map>

#include "../eventlog.h"
#include "../world.h"

namespace nullity {

	/// An implemented reality.
	class Reality : public IReality {
	public:
		IReality*				GetSource();

		/// Gets the time when the reality was created or split off from
		/// a source reality.
		TimeStep		GetStart();

		/// Gets the time when the reality was ended or split into another
		/// reality.
		TimeStep		GetEnd();

		/// Initializes a root reality.
		void	Init();

		/// Destroys this and all derived realities.
		void	Destroy();

		/// Splits off a new reality at the end of this reality.
		Reality*	Split();

		/// Cuts off the reality after the specified time and puts it
		/// into a new reality.
		Reality*	Cut(TimeStep Time);

		/// Merges this reality to its only derived reality. This can only
		/// be done if there is only one derived reality.
		void		Merge();

		/// Records the state of the object at the time in this reality.
		void		RecordState(TimeStep Time, IObject* Object);

		/// Records the removal of an object.
		void		RecordRemoval(TimeStep Time, IObject* Object);

	private:
		// Recorded entity state.
		struct _entity_state {
			TimeStep			Time;
			IFrame*				Recorded;
			IEntity*			Entity;
			IObject*			State;	// Null if removed.
			_entity_state*		Next;	// Next state for this entity.
		};

		// Division of time.
		struct _time_block {
			std::map<IEntity*, _entity_state*>		Entities;	// Map of entities that exist throughout the entire timeblock
																// and the state that corresponds to the the entity at the
																// begining of the block.
			std::map<IEntity*, _entity_state*>		Introduced; // Map of entities introduced in this time block and their
																// first state.
			std::map<IEntity*, _entity_state*>		Removed;	// Map of entities removed in this time block and their
																// last state.
			_time_block*							Prev;		// Last time block.
			_time_block*							Child;		// Smaller time block with same ending time.
			TimeStep								End;		// End of the time block.
		};

		Reality*								_source;
		std::vector<Reality*>					_derived;
		_time_block								_timeblock;

		void	_add_entity_state(_entity_state* State);
	};

}

#endif