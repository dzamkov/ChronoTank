#ifndef NULLITY_ENTITY_H
#define NULLITY_ENTITY_H

#include <string>
#include <set>

#include "object.h"
#include "time.h"

namespace nullity {
	class IVisual;
	struct VisualParameters;

	class IEntity;
	class IDynamicEntity;
	class IVisualEntity;
	struct EntityClass;
	
	/// An object in a world that affects and 
	/// interacts with other entities in the world.
	class IEntity : public virtual IObject {
	public:

		/// Copies all entity data to the target entity but does not 
		/// insert the entity into any frames. The default of this 
		virtual void				Clone(Ptr<IEntity> To);

		/// Gets the class of this entity. More specifically, this is the class
		/// that was used to create the entity or the interface that this entity
		/// has.
		virtual EntityClass*		GetClass() = 0;

		/// Returns a map of availibe interfaces to this entity paired with
		/// a pointer to the interface.
		virtual std::map<EntityClass*, Ptr<IEntity>>	GetInterfaces();
	};

	/// An interface to an entity which changes over time by itself.
	class IDynamicEntity : public IEntity {
	public:

		/// Updates the entity by the specified time with no interaction among
		/// other entities.
		virtual void		Update(TimeStep Time) = 0;

		EntityClass*		GetClass();

	};

	/// Represents a type of entity interface and provides methods to create an
	/// instance of the class. If create returns null the entity is abstract and
	/// can't be used directly.
	struct EntityClass {

		/// Gets the debug name for this class.
		virtual std::string			GetName();

		/// Creates an instance of this class.
		virtual Ptr<IEntity>		Create();
	};

	/// Fully copies the specified entity but does not insert it into
	/// a frame or reality.
	Ptr<IEntity>		Clone(Ptr<IEntity> Entity);

}

#endif