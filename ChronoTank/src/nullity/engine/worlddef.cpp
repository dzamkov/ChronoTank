/************************************************/
/*	World Implementation     					*/
/************************************************/
#include <limits>

#include "worlddef.h"

using namespace nullity;

/************************************************/
/*	World functions definitions     			*/
/************************************************/
//--
void MessageEvent::Remove() {
	delete this;
}

//--
void World::Init(IObject* Root) { 
	SetObjectEntity(Root, this->CreateEntity());
	this->_log.AddObjectStateEvent(
		-std::numeric_limits<TimeStep>::infinity(),
		Root, true, true);
}

//--
void World::Destroy() {
	// Destroy frames
	for(std::vector<Frame>::iterator it = this->_frames.begin();
		it != this->_frames.end(); it++)
	{
		(*it).Destroy();
	}
	// Destroy messages
	for(std::map<IMessage*, std::vector<MessageEx*>>::iterator it = this->_messageusages.begin();
		it != this->_messageusages.end(); it++)
	{
		(*it).first->Destroy();
	}
	delete this;
}

//--
IFrame* World::CreateOriginFrame() {
	Frame* f = this->CreateFrame();
	f->Init(this, 0.0f);
	return f;
}

//--
Entity* World::CreateEntity() {
	Entity e;
	this->_entities.push_back(e);
	return &(this->_entities.back());
}

//--
Frame*	World::CreateFrame() {
	Frame f;
	this->_frames.push_back(f);
	return &(this->_frames.back());
}

//--
MessageEx* World::CreateMessage() {
	MessageEx m;
	m.From = NULL;
	m.To = NULL;
	m.Message = NULL;
	m.Event = NULL;
	this->_messages.push_back(m);
	return &(this->_messages.back());
}

//--
void World::SendMessage(nullity::MessageEx* Message) {
	// Add usage for that message
	this->_messageusages[Message->Message].push_back(Message);

	// Create event
	MessageEvent* me = new MessageEvent();
	me->MessageData = Message;
	me->Flags = MessageEvent::Flag;
	me->Time = Message->Destination;
	Message->Event = me;

	// Add event
	this->_log.AddEvent(me);

	// Send skip event to frames
	for(std::vector<Frame>::iterator it = this->_frames.begin();
		it != this->_frames.end(); it++)
	{
		(*it).OnSentMessage(Message);
	}
}

//--
void World::CancelMessage(MessageEx* Message) {
	// Remove event
	this->_log.RemoveEvent(Message->Event);
	Message->Event = NULL;
}

//--
void World::LoadIntoFrame(TimeStep Time, Frame* Frame) {
	// Add objects based on events.
	std::vector<IObject*> objs = this->_log.GetObjectStatesAtTime(Time);
	for(std::vector<IObject*>::iterator it = objs.begin();
		it != objs.end(); it++)
	{
		Frame->AddObject(Clone(*it));
	}
}

//--
void World::RecordState(TimeStep Time, IObject* State) {
	this->_log.AddObjectStateEvent(Time, State, true, true);
}

//--
std::map<Entity*, std::vector<MessageEx*>> World::GetReceivedMessages(TimeStep Start, TimeStep End) {
	std::map<Entity*, std::vector<MessageEx*>> res;
	std::vector<const EventLog::Event*> messageevents = this->_log.GetEvents(Start, End, MessageEvent::Flag);
	for(std::vector<const EventLog::Event*>::iterator eit = messageevents.begin();
		eit != messageevents.end(); eit++)
	{
		const MessageEvent* me = (const MessageEvent*)(*eit);
		MessageEx* messagedata = me->MessageData;
		res[messagedata->To].push_back(messagedata);
	}
	return res;
}