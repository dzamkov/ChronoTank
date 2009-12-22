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

		// Partion of time.
		struct _time_block {
			_time_block*						Parent;		// Larger enclosing timeblock.
			_time_block*						Next;		// Next block or null if end or split.
			_time_block*						Child;		// Smaller enclosed timeblock that shares first state.
			_entity_state*						First;		// First state in this timeblock.
			std::map<IEntity*, _entity_state*>	FirstMap;	// First state for entities in this block. Can be outside
															// the time block if the entity exists in the block but the
															// last state update was in another block.
			IFrame*								Recorded;	// Frame that recorded this block, or null with multiple frames.
			int									States;		// Amount of state updates in the block.
		};

		Reality*					_source;
		std::vector<Reality*>		_derived;
		TimeStep					_end;
		_time_block*				_block;		// Block enclosing this reality.

		void	_add_entity_state(_entity_state* State);
	};

}

#endif