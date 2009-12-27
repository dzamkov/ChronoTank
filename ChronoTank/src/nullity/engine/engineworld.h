#ifndef NULLITY_ENGINE_WORLD_H
#define NULLITY_ENGINE_WORLD_H

#include <vector>
#include <map>

#include "enginereality.h"
#include "engineentity.h"
#include "engineframe.h"

#include "../world.h"

namespace nullity {
	class World;

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