#ifndef CTANK_OBJECT_H
#define CTANK_OBJECT_H

#include <irrlicht.h>

#include "visual.h"

namespace ctank {
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

		/// Called when the object needs to handle a message.
		virtual void	OnReceiveMessage(IMessageNotice* Notice);

		/// Called when a message was sent to the object, but that time has already passed
		/// for the object.
		virtual void	OnSkipMessage(IMessageNotice* Notice);

	protected:

		/// Sends a message to another entity. The message will be recevied by an object of
		/// the entity that is at the relative destination time. For example, if an object
		/// sends a message to another object with the destination of -5, the other object
		/// will get a OnReceiveMessage callback 5 seconds before the original object sent
		/// it. The source value of the callback will be 5, because that's the relative
		/// time when the message was sent.
		void			SendMessage(IEntity* Entity, TimeStep Destination, IMessage* Message);

		/// Sends a message to the entity of this object at the relative destination time. If
		/// the destination is negative, the object will reveive an OnSkipMessage callback for
		/// that message(This is because the sender should have actually received its own 
		/// message before it sent it).
		void			SendMessage(TimeStep Destination, IMessage* Message);

		/// Sends a message to this entity at this time. This is useful when the object
		/// has behavior not entirely defined by its state and interactions. This allows
		/// the object to repeat all previous "random" behavior. The message is not
		/// received on the object that sent it, and will only be received by another
		/// object of the same entity. The source value of the OnReceiveMessage callback
		/// will be 0 when it is received.
		void			SendMessage(IMessage* Message);

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
	
	/// A message that can be sent from any entity at any time period to any other
	/// entity at any other time.
	class IMessage {
	public:

		/// Deletes the message when it is no longer needed.
		virtual void			Destroy() = 0;

	};

	/// A message notice contains a message along with information of its sender,
	/// source and destination.
	class IMessageNotice {
	public:

		/// Gets the message contained in the notice.
		virtual IMessage*		GetMessage() = 0;	

		/// Gets the entity that sent the message.
		virtual IEntity*		GetSender() = 0;

		/// Gets the entity the message was meant for. This
		/// is most likely the entity that recieved the notice.
		virtual IEntity*		GetRecipient() = 0;

		/// Gets the time the message was sent relative to
		/// the time the notice was recieved.
		virtual TimeStep		GetSource() = 0;

		/// Gets the time the message is meant to be received relative
		/// to the time the notice was recieved. This is usually 0.
		virtual TimeStep		GetDestination() = 0;

		/// Prevents this message from being sent.
		virtual void			Cancel() = 0;
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