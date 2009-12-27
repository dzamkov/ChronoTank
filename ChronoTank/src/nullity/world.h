#ifndef NULLITY_WORLD_H
#define NULLITY_WORLD_H

#include <vector>

#include "object.h"
#include "visual.h"

namespace nullity {
	class IWorld;
	class IReality;
	class IFrame;

	/// Flags that show how a world or frame is seen.
	enum VisualFlags {
		VisualFlagNoCreation		= 0x00000001,
		VisualFlagFullDelete		= 0x00000002,
		VisualFlagAllowCreation		= 0x00000004,
		VisualFlagPerformRender		= 0x00000010,
		VisualFlagNoRender			= 0x00000020,
	};
		
	/// Organizes a set of objects and controls their interactions.
	class IWorld {
	public:

		/// Initializes this world. The root object will be created at
		/// the origin frame.
		virtual void		Init(IObject* Root) = 0;

		/// Gets the reality at the start of the world.
		virtual IReality*	GetRootReality() = 0;

		/// Destroys the world and all its resources.
		virtual void		Destroy() = 0;

		/// Gets a frame corresponding to the origin of the worlds time.
		virtual IFrame*		GetOriginFrame() = 0;
	};

	/// A possible progression of a world. For example, the events leading up
	/// to flipping a coin are in a single reality. Then, the one reality is split
	/// into both possible outcomes of the coin flip. The source of these realities
	/// is the original reality created before the coin was flipped.
	class IReality {
	public:

		/// Gets the reality this reality split off from. The source reality is actually
		/// part of this reality, however it is not a possible outcome and is instead the
		/// events leading up to the split.
		virtual IReality*					GetSource() = 0;

	};

	/// Represents a dynamic frame of time. If an IWorld is the representation
	/// of the eternal state of the scene, with information on everything that
	/// happened and the ability to predict what will happen, an IFrame is a
	/// representation of a present state of a scene, only knowing about what
	/// is presently there.
	class IFrame {
	public:

		/// Deletes the frame and all its resources. Its changes and effects
		/// are preserved in its world.
		virtual void		Destroy() = 0;

		/// Updates and advances the frame and its contents by the
		/// specified amount of time.
		virtual void		Update(TimeStep Time) = 0;

		/// Spawns an object in the frame, causing it to be created and added
		/// to the world on the next update.
		virtual void		SpawnObject(IObject* Object) = 0;

		/// Gets the object that represents the specified entity in this frame or returns NULL
		/// if the object doesnt exist in the frame.
		virtual IObject*	ObjectFor(IEntity* Entity) = 0;

		/// Sets the flags for the visual appearence of the frame. These
		/// flags should be from VisualFlags.
		virtual void		SetVisualFlags(int Flags) = 0;

		/// Sets the parameters for which visuals are created. This only
		/// matters if the right visual flags are set.
		virtual void		SetVisualParameters(VisualParameters Params) = 0;

		/// Renders visuals if the right visual flags are set.
		virtual void		RenderVisuals() = 0;
	};

	/// Creates a world.
	IWorld*		CreateWorld();

}

#endif