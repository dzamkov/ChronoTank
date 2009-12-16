/************************************************/
/*	Event log			     					*/
/************************************************/
#include <map>

#include "eventlog.h"

using namespace ctank;

/****************************************/
/*	Event log implementation			*/
/****************************************/
//--
EventLog::EventLog() {

}

//--
EventLog::~EventLog() {
	for(std::vector<Event*>::iterator it = this->_events.begin();
		it != this->_events.end(); it++)
	{
		Event* e = *it;
		e->Remove();
	}
}

//--
void EventLog::RemoveEvents(TimeStep Start, TimeStep End, int Flags) {
	// List of events to remove.
	std::vector<std::vector<Event*>::iterator> toremove;

	// Mark events for deletion.
	for(std::vector<Event*>::iterator it = this->_events.begin();
		it != this->_events.end(); it++)
	{
		Event* e = *it;
		if((e->Flags & Flags) && e->Time >= Start && e->Time <= End) {
			e->Remove();
			toremove.push_back(it);
		}
	}

	// Remove events
	for(std::vector<std::vector<Event*>::iterator>::iterator eit = toremove.begin();
		eit != toremove.end(); eit++)
	{
		this->_events.erase(*eit);
	}
}

//--
void EventLog::AddEvent(EventLog::Event* Event) {
	this->_events.push_back(Event);
}

//--
void EventLog::RemoveEvent(EventLog::Event* E) {
	for(std::vector<Event*>::iterator it = this->_events.begin();
		it != this->_events.end(); it++)
	{
		Event* e = *it;
		if(e == E) {
			e->Remove();
			this->_events.erase(it);
		}
	}
}

//--
std::vector<const EventLog::Event*> EventLog::GetEvents(TimeStep Start, TimeStep End, int Flags) {
	std::vector<const Event*> result;
	for(std::vector<Event*>::iterator it = this->_events.begin();
		it != this->_events.end(); it++)
	{
		Event* e = *it;
		if((e->Flags & Flags) && e->Time >= Start && e->Time <= End) {
			result.push_back(e);
		}
	}
	return result;
}

//--
IObject* EventLog::GetObjectStateAtTime(IEntity* Entity, TimeStep Time) {
	IObject* state = NULL;
	TimeStep statetime = 0.0f;
	for(std::vector<Event*>::iterator it = this->_events.begin();
		it != this->_events.end(); it++)
	{
		Event* e = *it;
		if(e->Flags & ObjectStateEvent::Flag) {
			ObjectStateEvent* ose = (ObjectStateEvent*)(*it);
			if(ose->Entity == Entity && ose->Time < Time && (state == NULL || ose->Time > statetime)) {
				state = ose->State;
				statetime = ose->Time;
			}
		}
	}
	return state;
}

//--
std::vector<IObject*> EventLog::GetObjectStatesAtTime(TimeStep Time) {
	// Definition of a state for storage in a map.
	struct stateinfo {
		IObject*	state;
		TimeStep	time;
	};

	// Map of object states.
	std::map<IEntity*, stateinfo> states;

	// Create history of objects.
	for(std::vector<Event*>::iterator it = this->_events.begin();
		it != this->_events.end(); it++)
	{
		Event* e = *it;
		TimeStep time = e->Time;
		if(time < Time) {
			if(e->Flags & ObjectStateEvent::Flag) {
				// Update state if needed.
				ObjectStateEvent* ose = (ObjectStateEvent*)(*it);
				IObject* state = ose->State;
				IEntity* entity = ose->Entity;
				stateinfo si; si.state = state; si.time = time;
				std::map<IEntity*, stateinfo>::iterator mapstate = states.find(entity);
				if(mapstate != states.end()) {
					if(time > (*mapstate).second.time) {
						(*mapstate).second = si;
					}
				} else {
					states[entity] = si; 
				}
			}
			if(e->Flags & ObjectRemovalEvent::Flag) {
				// Remove info.
				ObjectRemovalEvent* ore = (ObjectRemovalEvent*)(*it);
				states.erase(ore->Entity);
			}
		}
	}

	// Convert to state form.
	std::vector<IObject*> result;
	for(std::map<IEntity*, stateinfo>::iterator mit = states.begin();
		mit != states.end(); mit++) 
	{
		result.push_back((*mit).second.state);
	}
	return result;
}

//--
void EventLog::AddObjectStateEvent(TimeStep Time, IObject* Object, bool Clone, bool AutoDelete) {
	ObjectStateEvent* ose = new ObjectStateEvent();
	ose->Entity = Object->GetEntity();
	ose->State = Clone ? (ctank::Clone(Object)) : Object;
	ose->Time = Time;
	ose->Flags = AutoDelete ? ObjectStateEvent::Flag | ObjectStateEvent::AutoDeleteFlag : ObjectStateEvent::Flag;
	this->AddEvent(ose);
}

//--
void EventLog::Event::Remove() {
	delete this;
}

//--
void EventLog::ObjectStateEvent::Remove() {
	if(this->Flags & AutoDeleteFlag) {
		delete this->State;
	}
	delete this;
}

//--
void EventLog::ObjectRemovalEvent::Remove() {
	delete this;
}