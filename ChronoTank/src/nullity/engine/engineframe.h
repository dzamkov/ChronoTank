#ifndef NULLITY_ENGINE_FRAME_H
#define NULLITY_ENGINE_FRAME_H

#include <vector>
#include <map>

#include "enginereality.h"
#include "engineentity.h"

#include "../visual.h"
#include "../object.h"
#include "../world.h"

namespace nullity {
	class World;
	class Frame;
	struct ObjectEx;

	/// An extended object containing visual information.
	struct ObjectEx {
		IObject*		Object;
		IVisual*		Visual;

		/// Destroys the object visual pair.
		void		Destroy();

		/// Manages the visual aspects of the object.
		void		Manage(int Flags, const VisualParameters& Params);

		/// Updates the pair.
		void		Update(TimeStep Time, int Flags);
	};

	/// An implemented interface to a frame that uses World.
	class Frame : public IFrame {
	public:
		
		/// Creates a frame.
		void		Init(TimeStep Time, Reality* Reality, bool Write);

		/// Destroys the frame and its resources.
		void		Destroy();

		/// Directly adds an object to the frame.
		void		AddObject(IObject* Object);

		/// Callback for when this frames reality is destroyed.
		void		OnRealityDestroyed();

		IObject*	ObjectFor(IEntity* Entity);
		void		Update(TimeStep Time);
		void		SpawnObject(IObject* Object);
		void		SetVisualFlags(int Flags);
		void		SetVisualParameters(VisualParameters Params);
		void		RenderVisuals();

	private:
		VisualParameters					_visparams;
		int									_visflags;
		std::map<Entity*, ObjectEx>			_objects;

		// List of accumulated tasks for the frame.
		struct _tasklist {
			std::vector<IObject*>		SpawnedObjects;

			// Performs tasks.
			void	PerformTasks(Frame*	Frame);
		} _tasks;

		// Swaps the frame to another reality. Can only be called
		// by the current reality.
		void			_swap_reality(Reality* Reality);

		// Time the frame is in.
		TimeStep		_time;

		// Reality this frame reads/writes
		Reality*		_reality;

		// Should the frame write to the reality.
		bool			_write;

		// World where the frames resources come from.
		World*			_world;

		friend class		World;
		friend class		Reality;
	};
}

#endif