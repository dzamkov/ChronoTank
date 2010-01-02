#ifndef NULLITY_ENGINE_WORLD_H
#define NULLITY_ENGINE_WORLD_H

#include <vector>
#include <map>

#include "enginereality.h"
#include "engineframe.h"

#include "../world.h"

namespace nullity {
	class World;

	/// An implemented interface to a world.
	class World : public IWorld {
	public:
		void		Init(StackPtr<IEntity> Root);

		StackPtr<IFrame>		GetOriginFrame();
		StackPtr<IReality>		GetRootReality();

	private:
		Ptr<Reality>				_rootreality;
	};


}

#endif