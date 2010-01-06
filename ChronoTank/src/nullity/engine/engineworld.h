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
		World();

		void		Init(Ptr<Entity> Root);

		Ptr<IFrame>		GetOriginFrame();
		Ptr<IReality>	GetRootReality();

	private:
		Ptr<Reality>				_rootreality;
	};


}

#endif