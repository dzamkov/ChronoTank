/************************************************/
/*	Frame Implementation     					*/
/************************************************/
#include "framedef.h"
#include "worlddef.h"

using namespace nullity;

/************************************************/
/*	Frame functions definitions     			*/
/************************************************/
//--
TimeStep MessageNotice::GetDestination() {
	return this->Destination;
}

//--
TimeStep MessageNotice::GetSource() {
	return this->Source;
}

//--
IEntity* MessageNotice::GetSender() {
	return this->MessageData->From;
}

//--
IEntity* MessageNotice::GetRecipient() {
	return this->MessageData->To;
}

//--
IMessage* MessageNotice::GetMessage() {
	return this->MessageData->Message;
}

//--
void MessageNotice::Cancel() {
	this->Frame->_tasks.CancelledMessages.push_back(this->MessageData);
}

//--
void ObjectEx::Destroy() {
	this->Object->Destroy();
	if(this->Visual != NULL) {
		this->Visual->Destroy();
	}
}

//--
void ObjectEx::Manage(int Flags, const VisualParameters& Params) {
	if(this->Visual == NULL) {
		if(Flags & VisualFlagFullDelete) {
			this->Visual->Destroy();
			this->Visual = NULL;
		}
	} else {
		if(Flags & VisualFlagAllowCreation) {
			this->Visual = this->Object->CreateVisual(Params);
		}
	}
}

//--
void ObjectEx::Update(TimeStep Time, int Flags) {
	this->Object->Update(Time);
	if(this->Visual != NULL) {
		this->Visual->Update(Time);
	}
}

//--
void Frame::Init(World* World, TimeStep Time) {
	// Set frame values
	this->_world = World;
	this->_time = Time;

	// Set initial flags
	this->SetVisualFlags(VisualFlagNoCreation | VisualFlagNoRender);

	// Load data
	this->_world->LoadIntoFrame(this->_time, this);
}

//--
void Frame::Destroy() {
	for(std::map<Entity*, ObjectEx>::iterator it = this->_objects.begin();
		it != this->_objects.end(); it++)
	{
		(*it).second.Destroy();
	}
}

//--
void Frame::Update(TimeStep Time) {
	// Timing
	TimeStep ustart = this->_time;
	TimeStep uend = ustart + Time;

	// Tasks
	this->_tasks.PerformTasks(this);

	// Messages
	std::map<Entity*, std::vector<MessageEx*>> messages = this->_world->GetReceivedMessages(ustart, uend);
	for(std::map<Entity*, std::vector<MessageEx*>>::iterator mit = messages.begin();
		mit != messages.end(); mit++)
	{
		std::vector<MessageEx*>& ms = (*mit).second;
		IObject* obj = this->ObjectForEntity((*mit).first);
		if(obj) {
			for(std::vector<MessageEx*>::iterator mmit = ms.begin();
				mmit != ms.end(); mmit++)
			{
				MessageEx* messagedata = *mmit;
				MessageNotice mn;
				mn.Frame = this;
				mn.MessageData = messagedata;
				mn.Destination = messagedata->Destination - ustart;
				mn.Source = messagedata->Source - ustart;
				obj->OnReceiveMessage(&mn);
			}
		}
	}

	// Object updates
	for(std::map<Entity*, ObjectEx>::iterator it = this->_objects.begin();
		it != this->_objects.end(); it++)
	{
		ObjectEx& obj = (*it).second;
		TimeStep utime = Time * this->GetTimeRateForObject(obj.Object);
		obj.Manage(this->_visflags, this->_visparams);
		obj.Update(utime, this->_visflags);
		this->_world->RecordState(uend, obj.Object);
	}

	// Status
	this->_time = uend;
}

//--
void Frame::UpdateForObject(IObject* Object, TimeStep Time) {
	this->Update(Time / this->GetTimeRateForObject(Object));
}

//--
void Frame::SpawnObject(IObject* Object) {
	SetObjectEntity(Object, this->_world->CreateEntity());
	this->_tasks.SpawnedObjects.push_back(Object);
}

//--
void Frame::AddObject(IObject* Object) {
	ObjectEx objex; objex.Object = Object;
	SetObjectFrame(objex.Object, this);
	objex.Manage(this->_visflags, this->_visparams);
	this->_objects[(Entity*)(objex.Object->GetEntity())] = objex;
}

//--
TimeStep Frame::GetTimeRateForObject(IObject* Object) {
	// TODO: Time manipulators
	return 1.0f;
}

//--
IWorld* Frame::GetWorld() {
	return this->_world;
}

//--
void Frame::SetVisualFlags(int Flags) {
	this->_visflags = Flags;
}

//--
void Frame::SetVisualParameters(VisualParameters Params) {
	this->_visparams = Params;
}

//--
void Frame::RenderVisuals() {
	if(this->_visflags & VisualFlagPerformRender) {
		for(std::map<Entity*, ObjectEx>::iterator it = this->_objects.begin();
			it != this->_objects.end(); it++)
		{
			ObjectEx& obj = (*it).second;
			if(obj.Visual != NULL) {
				obj.Visual->Render();
			}
		}
	}
}

//--
void Frame::SendMessage(IObject* From, IEntity* To, TimeStep Offset, IMessage* Message) {
	// Set message parameters.
	TimeStep time = this->_time;
	MessageEx* me = this->_world->CreateMessage();
	me->Destination = time + Offset;
	me->From = (Entity*)From->GetEntity();
	me->To = (Entity*)To;
	me->Source = time;
	me->Message = Message;
	
	// Add to outbox.
	this->_tasks.SentMessages.push_back(me);
}

//--
void Frame::OnSentMessage(MessageEx* Message) {
	TimeStep time = this->_time;

	// If its after the time the message was intended for
	if(time > Message->Destination) {

		// Find the recipient.
		IObject* rec = this->ObjectForEntity(Message->To);
		if(rec) {
			MessageNotice mn;
			mn.Frame = this;
			mn.MessageData = Message;
			mn.Destination = Message->Destination - time;
			mn.Source = Message->Source - time;
			rec->OnSkipMessage(&mn);
		}
	}
}

//--
IObject* Frame::ObjectForEntity(Entity* E) {
	std::map<Entity*, ObjectEx>::iterator ent = this->_objects.find(E);
	if(ent != this->_objects.end()) {
		return (*ent).second.Object;
	} else {
		return NULL;
	}
}

//--
void Frame::_tasklist::PerformTasks(Frame* Frame) {
	// Send out messages
	for(std::vector<MessageEx*>::iterator mit = this->SentMessages.begin();
		mit != this->SentMessages.end(); mit++)
	{
		Frame->_world->SendMessage(*mit);
	}

	// Cancel messages
	for(std::vector<MessageEx*>::iterator mit = this->CancelledMessages.begin();
		mit != this->CancelledMessages.end(); mit++)
	{
		Frame->_world->CancelMessage(*mit);
	}

	// Spawning
	for(std::vector<IObject*>::iterator oit = this->SpawnedObjects.begin();
		oit != this->SpawnedObjects.end(); oit++)
	{
		Frame->AddObject(*oit);
	}
}