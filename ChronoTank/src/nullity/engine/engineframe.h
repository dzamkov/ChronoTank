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

		Ptr<IEntity>		Entity;
		Ptr<IVisual>		Visual;

		/// Destroys the entity visual pair.
		void		Destroy();

		/// Manages the visual aspects of the object.
		void		Manage(int Flags, const VisualParameters& Params);

		/// Updates the pair.
		void		Update(TimeStep Time, int Flags);
	};

	/// An implemented interface to a frame that uses World.
	class Frame : public IFrame {
	public:
		Frame();
		
		/// Creates a frame.
		void		Init(TimeStep Time, StackPtr<Reality> Reality, bool Write);

		/// Destroys the frame and its resources.
		void		Destroy();

		/// Directly adds an entity to the frame.
		void		AddEntity(StackPtr<IEntity> Entity);

		/// Callback for when this frames reality is destroyed.
		void		OnRealityDestroyed();

		void		Update(TimeStep Time);
		void		SpawnEntity(StackPtr<IEntity> Entity);
		void		SetVisualFlags(int Flags);
		void		SetVisualParameters(VisualParameters Params);
		void		RenderVisuals();

	private:
		typedef	std::map<Ptr<IEntity>, Ptr<EntityEx>>	_entitymap;

		VisualParameters		_visparams;
		int						_visflags;
		_entitymap				_entities;

		// List of accumulated tasks for the frame.
		struct _tasklist {
			std::vector<Ptr<IEntity>>		SpawnedEntities;

			// Performs tasks.
			void	PerformTasks(Frame* Frame);
		} _tasks;

		// Swaps the frame to another reality. Can only be called
		// by the current reality.
		void			_swap_reality(StackPtr<Reality> Reality);

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