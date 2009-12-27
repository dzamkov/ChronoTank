#ifndef NULLITY_OBJECT_H
#define NULLITY_OBJECT_H

#include <irrlicht.h>

#include "time.h"
#include "visual.h"

namespace nullity {
	class IWorld;
	class IFrame;
	class IObject;
	class IMessage;
	class IMessageNotice;
	class IEntity;

	/// Defines a vector.
	typedef irr::core::vector3df			Vector;

	/// Defines a timestep.
	typedef	float							TimeStep;

	/// The base class for everthing that can affect and can be
	/// seen in a world.
	class IObject {
	public:
		IObject();

		/// Updates the object by the specified amount of seconds.
		virtual void			Update(TimeStep Time);

		/// Removes the resources used for the object. Should be called instead
		/// of delete. 
		virtual void			Destroy() = 0;
		
		/// Gets the position of the center of the object.
		virtual Vector			GetPosition();

		/// Gets the rotation of the object.
		virtual Vector			GetRotation();

		/// Gets the frame this object is in. If this returns NULL, the
		/// object is in a static phase, where it can't interact or update.
		IFrame*					GetFrame();

		/// Gets the entity this object represents. This is NULL only when
		/// the object is not assigned to a world. Static objects may or may
		/// not have entities. An object with a particular entity will never
		/// be in the same frame with an object with the same entity.
		IEntity*				GetEntity();

		/// Creates a visual that represents this object.
		virtual IVisual*		CreateVisual(VisualParameters Params);

		/// Gets a flag that indicates if this object has
		/// special properties and should be treated differently.
		/// The default value is 0. This value should only
		/// be changed if the normal mechanism for objects can
		/// not correctly represent it.
		virtual int				GetSpecialFlag();

		/// Creates a copy of the object in its current state, only
		/// removing frame and entity information. The two objects should
		/// then be independant.
		virtual IObject*		Clone();

	private:
		IFrame*					_frame;
		IEntity*				_entity;
		friend void				SetObjectEntity(IObject*, IEntity*);
		friend void				SetObjectFrame(IObject*, IFrame*);
		friend IObject*			Clone(IObject*);
	};

	/// Represents an objects existance over time. An object is just a slice
	/// of an entity at a particular time. An entity itself specifies no behavior
	/// of an object, only its state over time. Entities are managed by the world
	/// the object is in.
	class IEntity {
	public:

	};

	/// Creates a clone of the specified object preserving entity information.
	IObject*		Clone(IObject* Object);

	/// Sets the entity of an object. This should never be done by the object itself, and is
	/// not included in the object's definition for that reason.
	void			SetObjectEntity(IObject* Object, IEntity* Entity);

	/// Sets the frame of an object. This should never be done by the object itself, and is
	/// not included in the object's definition for that reason.
	void			SetObjectFrame(IObject* Object, IFrame* Frame);
}

#endif