#ifndef NULLITY_ENGINE_REALITY_H
#define NULLITY_ENGINE_REALITY_H

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

	private:
		Reality*				_source;
		std::vector<Reality*>	_derived;
		TimeStep				_end;
	};

}

#endif