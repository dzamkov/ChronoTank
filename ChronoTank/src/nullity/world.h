#ifndef NULLITY_WORLD_H
#define NULLITY_WORLD_H

#include "object.h"
#include "visual.h"

namespace nullity {
	class IWorld;
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

		/// Destroys the world and all its resources.
		virtual void		Destroy() = 0;

		/// Creates a frame corresponding to the origin of the worlds time.
		virtual IFrame*		CreateOriginFrame() = 0;
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

		/// Updates the frame for an amount of time so that the object
		/// experiences the specified amount of time.
		virtual void		UpdateForObject(IObject* Object, TimeStep Time) = 0;

		/// Spawns an object in the frame, causing it to be created and added
		/// to the world on the next update.
		virtual void		SpawnObject(IObject* Object) = 0;

		/// Gets the rate at which time passes for the specified object, relative
		/// to absoulte time. The object must exist in this frame.
		virtual TimeStep	GetTimeRateForObject(IObject* Object) = 0;

		/// Gets the world this frame is in.
		virtual IWorld*		GetWorld() = 0;

		/// Sends a message to an entity at a certain offset from the frame's time.
		virtual void		SendMessage(IObject* From, IEntity* To, TimeStep Offset, IMessage* Message) = 0;

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