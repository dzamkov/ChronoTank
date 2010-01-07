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
	class IDynamicEntity;
	
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
		DECLARE_INTERFACE_CLASS(IEntityInterface)

		/// Copies this entity into a new entity preserving all
		/// internal information about the entity while leaving out
		/// usage information.
		virtual Ptr<Entity>		Clone() = 0;
	};

	/// Interface to an entity that changes over time by itself.
	class IDynamicEntity : public IInterface {
	public:
		DECLARE_INTERFACE_CLASS(IDynamicEntity)

		/// Updates the entity by the specified amount of time
		/// with no interaction with other entites.
		virtual void		Update(TimeStep Time) = 0;
	};

	/// An entity for which a visual representation can be created.
	class IVisualEntity : public IInterface {
	public:
		DECLARE_INTERFACE_CLASS(IVisualEntity)

		/// Creates the visual for this entity.
		virtual Ptr<IVisual>	CreateVisual(VisualParameters Params) = 0;
	};

	/// Clones an entity.
	Ptr<Entity>		Clone(Ptr<Entity> Entity);

}

#endif