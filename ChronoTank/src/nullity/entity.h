#ifndef NULLITY_ENTITY_H
#define NULLITY_ENTITY_H

#include <string>
#include <set>

#include "object.h"
#include "interface.h"
#include "time.h"

namespace nullity {
	class IVisual;
	struct VisualParameters;

	class Entity;
	class IEntityInterface;
	
	/// An object in a world that affects and 
	/// interacts with other entities in the world.
	class Entity : public IObject {
	public:

		void		Destroy();

		/// Initializes this entity with an interface to act as its
		/// main interface.
		void		Init(IInterface* MainInterface);

		/// Clones this entity.
		Ptr<Entity>			Clone();

		/// The main interface that defines the behavior of
		/// this entity.
		IInterface*			MainInterface;

		/// The entity interface for this entity. This is the
		/// base of main interface with the EntityInterface class.
		IEntityInterface*	EntityInterface;

	};

	/// Interface for an entity. All interfaces that intend
	/// to be the main interface of an entity must have this
	/// as a base.
	class IEntityInterface : public IInterface {
	public:
		static InterfaceClass*	Class;

		/// Copies this entity into a new entity preserving all
		/// internal information about the entity while leaving out
		/// usage information.
		virtual Ptr<Entity>		Clone() = 0;

	};

}

#endif