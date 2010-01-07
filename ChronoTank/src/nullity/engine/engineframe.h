#ifndef NULLITY_ENGINE_FRAME_H
#define NULLITY_ENGINE_FRAME_H

#include <vector>
#include <map>

#include "enginereality.h"

#include "../visual.h"
#include "../entity.h"
#include "../object.h"
#include "../world.h"

namespace nullity {
	class World;
	class Frame;
	struct EntityEx;

	/// An extended entity containing visual information.
	struct EntityEx : public IObject {
		EntityEx();

		Ptr<Entity>			Ent;
		Ptr<IVisual>		Visual;
		IVisualEntity*		VisualInt;
		IDynamicEntity*		DynamicInt;

		/// Sets the entity this represents.
		void		SetEntity(Ptr<Entity> Entity);

		/// Destroys the entity visual pair.
		void		Destroy();

		/// Manages the visual aspects of the entity.
		void		Manage(int Flags, const VisualParameters& Params);

		/// Updates the visual aspects of the entity.
		void		Update(int Flags);
	};

	/// An implemented interface to a frame that uses World.
	class Frame : public IFrame {
	public:
		Frame();
		
		/// Creates a frame.
		void		Init(TimeStep Time, Ptr<Reality> Reality, bool Write);

		/// Destroys the frame and its resources.
		void		Destroy();

		/// Directly adds an entity to the frame.
		void		AddEntity(Ptr<Entity> Entity);

		/// Callback for when this frames reality is destroyed.
		void		OnRealityDestroyed();

		void		Update(TimeStep Time);
		void		SpawnEntity(Ptr<Entity> Entity);
		void		SetVisualFlags(int Flags);
		void		SetVisualParameters(VisualParameters Params);
		void		RenderVisuals();

	private:
		typedef	std::map<Ptr<Entity>, Ptr<EntityEx>>	_entitymap;

		VisualParameters		_visparams;
		int						_visflags;
		_entitymap				_entities;

		// List of accumulated tasks for the frame.
		struct _tasklist {
			std::vector<Ptr<Entity>>		SpawnedEntities;

			// Performs tasks.
			void	PerformTasks(Frame* Frame);
		} _tasks;

		// Swaps the frame to another reality. Can only be called
		// by the current reality.
		void			_swap_reality(Ptr<Reality> Reality);

		// Time the frame is in.
		TimeStep		_time;

		// Reality this frame reads/writes
		Ptr<Reality>	_reality;

		// Should the frame write to the reality.
		bool			_write;

		// World where the frames resources come from.
		Ptr<World>		_world;

		friend class		World;
		friend class		Reality;
	};
}

#endif