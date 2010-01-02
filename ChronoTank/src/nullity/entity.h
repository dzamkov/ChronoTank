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
	struct EntityClass;
	
	/// An object in a world that affects and interacts with other entities
	/// in the world.
	class IEntity : public IObject {
	public:

		/// Copies all entity data to the target entity but does not 
		/// insert the entity into any frames.
		virtual void				Clone(StackPtr<IEntity>::Ref To) = 0;

		/// Advances the entity by the specified amount of time. No interaction
		/// with other entities should be done during this method.
		virtual void				Update(TimeStep Time) = 0;

		/// Creates a visual for this entity or returns null if the entity cant be
		/// visually represented.
		virtual StackPtr<IVisual>	CreateVisual(VisualParameters Params) = 0;

		/// Gets the class of this entity.
		virtual EntityClass*		GetClass() = 0;
		
	};

	/// Represents a type of entity and provides methods to create an
	/// instance of the class.
	struct EntityClass {

		/// Gets the debug name for this class.
		virtual std::string				GetName();

		/// Creates an instance of this class.
		virtual StackPtr<IEntity>		Create();
	};

	/// Fully copies the specified entity but does not insert it into
	/// a frame or reality.
	StackPtr<IEntity>		Clone(StackPtr<IEntity>::Ref Entity);

}

#endif