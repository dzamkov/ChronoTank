#ifndef NULLITY_ENGINE_FRAME_H
#define NULLITY_ENGINE_FRAME_H

#include <vector>
#include <map>

#include "engineentity.h"

#include "../visual.h"
#include "../object.h"
#include "../world.h"

namespace nullity {
	class World;
	class Frame;
	struct MessageEx;
	struct ObjectEx;

	/// An implementation of a message notice.
	class MessageNotice : public IMessageNotice {
	public:
		MessageEx*		MessageData;
		TimeStep		Source;
		TimeStep		Destination;
		Frame*			Frame;

		IMessage*		GetMessage();	
		IEntity*		GetSender();
		IEntity*		GetRecipient();
		TimeStep		GetSource();
		TimeStep		GetDestination();
		void			Cancel();
	};

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
		/// Initializes the frame with the specified world
		/// and begining timestep.
		void		Init(World* World, TimeStep Time);

		/// Destroys the frame and its resources.
		void		Destroy();

		/// Directly adds an object to the frame.
		void		AddObject(IObject* Object);

		/// Callback for when a new message is sent. This is
		/// used to invoke OnSkipMessage on an object if needed.
		void		OnSentMessage(MessageEx* Message);

		/// Gets the object for the specified entity or returns null
		/// if the object doesnt exist for the entity.
		IObject*	ObjectForEntity(Entity* Entity);

		void		Update(TimeStep Time);
		void		UpdateForObject(IObject* Object, TimeStep Time);
		void		SpawnObject(IObject* Object);
		TimeStep	GetTimeRateForObject(IObject* Object);
		IWorld*		GetWorld();
		void		SendMessage(IObject* From, IEntity* To, TimeStep Offset, IMessage* Message);
		void		SetVisualFlags(int Flags);
		void		SetVisualParameters(VisualParameters Params);
		void		RenderVisuals();

	private:
		World*								_world;
		VisualParameters					_visparams;
		int									_visflags;
		std::map<Entity*, ObjectEx>			_objects;

		// List of accumulated tasks for the frame.
		struct _tasklist {
			std::vector<MessageEx*>		SentMessages;
			std::vector<MessageEx*>		CancelledMessages;
			std::vector<IObject*>		SpawnedObjects;

			// Performs tasks.
			void	PerformTasks(Frame*	Frame);
		} _tasks;

		// Time the frame is in.
		TimeStep		_time; 

		friend class		MessageNotice;
	};
}

#endif