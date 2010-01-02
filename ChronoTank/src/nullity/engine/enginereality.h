#ifndef NULLITY_ENGINE_REALITY_H
#define NULLITY_ENGINE_REALITY_H

#include <vector>
#include <set>
#include <map>

#include "../world.h"

namespace nullity {
	class Reality;
	class Frame;

	/// An implemented reality.
	class Reality : public IReality {
	public:
		StackPtr<IReality>			GetSource();

		/// Gets the time when the reality was created or split off from
		/// a source reality.
		TimeStep	GetStart();

		/// Gets the time when the reality was ended or split into another
		/// reality.
		TimeStep	GetEnd();

		/// Initializes a root reality with an accompanant write frame.
		void		Init(TimeStep Origin, StackPtr<Frame> WriteFrame);

		/// Destroys this and all derived realities.
		void		Destroy();

		/// Splits off a new reality at the end of this reality.
		StackPtr<Reality>	Split();

		/// Cuts off the reality after the specified time and puts it
		/// into a new reality.
		StackPtr<Reality>	Cut(TimeStep Time);

		/// Merges this reality to its only derived reality. This can only
		/// be done if there is only one derived reality.
		void				Merge();

		/// Records the state change from the entities old state to the entities new state
		/// at the given time. If the old state is null, the entity is created; likewise, if
		/// the new state is null, it is destroyed.
		void				RecordState(TimeStep Time, StackPtr<IEntity> Old, StackPtr<IEntity> New);

		/// Creates a read frame at the specified time.
		void				Read(TimeStep Time, StackPtr<Frame> WriteFrame);

		/// Creates a write frame at the specified time and sets it to write to
		/// a new reality.
		void				Fork(TimeStep Time, StackPtr<Frame> WriteFrame);

		/// Gets the frame that writes to this reality.
		StackPtr<Frame>		GetWriteFrame();

	private:

		TimeStep				_end;
		Ptr<Reality>			_source;
		std::set<Ptr<Reality>>	_derived;
		std::set<Ptr<Frame>>	_frames;
		Ptr<Frame>				_writeframe;

		// Removes a frame from this reality. Can only be done by the
		// frame.
		void					_remove_frame(StackPtr<Frame> Frame);

		friend class			Frame;
	};

}

#endif