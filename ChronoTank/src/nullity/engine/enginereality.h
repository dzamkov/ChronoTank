#ifndef NULLITY_ENGINE_REALITY_H
#define NULLITY_ENGINE_REALITY_H

#include <vector>
#include <set>
#include <map>

#include "../eventlog.h"
#include "../world.h"

namespace nullity {
	class Reality;
	class Frame;

	/// An implemented reality.
	class Reality : public IReality {
	public:
		IReality*				GetSource();

		/// Gets the time when the reality was created or split off from
		/// a source reality.
		TimeStep	GetStart();

		/// Gets the time when the reality was ended or split into another
		/// reality.
		TimeStep	GetEnd();

		/// Initializes a root reality with an accompanant write frame.
		void		Init(TimeStep Origin, Frame* WriteFrame);

		/// Destroys this and all derived realities.
		void		Destroy();

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

		/// Creates a read frame at the specified time.
		void		Read(TimeStep Time, Frame* WriteFrame);

		/// Creates a write frame at the specified time and sets it to write to
		/// a new reality.
		void		Fork(TimeStep Time, Frame* WriteFrame);

		/// Gets the frame that writes to this reality.
		Frame*		GetWriteFrame();

	private:

		// Object update information.
		struct _info {
			IObject*		State;
			TimeStep		Time;
		};

		typedef		std::vector<_info>				_histtype;
		typedef		std::map<IEntity*, _histtype>	_datatype;

		TimeStep				_end;
		Reality*				_source;
		std::set<Reality*>		_derived;
		std::set<Frame*>		_frames;
		Frame*					_writeframe;
		_datatype				_data;

		// Removes a frame from this reality. Can only be done by the
		// frame.
		void					_remove_frame(Frame* Frame);

		friend class			Frame;
	};

}

#endif