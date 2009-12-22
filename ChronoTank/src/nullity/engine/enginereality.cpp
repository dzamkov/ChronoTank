/************************************************/
/*	Reality Implementation     					*/
/************************************************/
#include <assert.h>
#include <limits>

#include "enginereality.h"

using namespace nullity;

/************************************************/
/*	Reality functions definitions     			*/
/************************************************/
//--
IReality* Reality::GetSource() { 
	return this->_source;
}

//--
TimeStep Reality::GetStart() {
	if(this->_source == NULL) {
		return -std::numeric_limits<TimeStep>::infinity();
	} else {
		return this->_source->_end;
	}
}

//--
TimeStep Reality::GetEnd() {
	return this->_end;
}

//--
void Reality::Init() {
	this->_source = NULL;
	this->_end = -std::numeric_limits<TimeStep>::infinity();
	this->_block = NULL;
}

//--
void Reality::Destroy() {
	for(std::vector<Reality*>::iterator it = this->_derived.begin();
		it != this->_derived.end(); it++)
	{
		(*it)->Destroy();
	}
	delete this;
}

//--
Reality* Reality::Split() {
	Reality* nr = new Reality();
	nr->_end = this->_end;
	nr->_source = this;
	this->_derived.push_back(nr);
	return nr;
}

//--
Reality* Reality::Cut(TimeStep Time) {
	assert(Time > this->GetStart() && Time <= this->GetEnd());
	Reality* nr = new Reality();
	nr->_end = this->_end;
	nr->_source = this;
	this->_derived.push_back(nr);
	this->_end = Time;
	return nr;
}

//--
void Reality::Merge() {
	if(this->_derived.size() == 1) {
		Reality* r = *(this->_derived.begin());
		this->_derived.empty();
		this->_end = r->_end;
		for(std::vector<Reality*>::iterator it = r->_derived.begin();
			it != r->_derived.end(); it++)
		{
			Reality* d = *it;
			d->_source = this;
			this->_derived.push_back(d);
		}
		delete r;
	}
}

//--
void Reality::RecordState(TimeStep Time, IObject* Object) {
	_entity_state* es = new _entity_state();
	es->Entity = Object->GetEntity();
	es->Recorded = Object->GetFrame();
	es->Time = Time;
	es->State = Clone(Object);
	this->_add_entity_state(es);
}

//--
void Reality::RecordRemoval(TimeStep Time, IObject* Object) {
	_entity_state* es = new _entity_state();
	es->Entity = Object->GetEntity();
	es->Recorded = Object->GetFrame();
	es->Time = Time;
	es->State = NULL;
	this->_add_entity_state(es);
}

//--
void Reality::_add_entity_state(Reality::_entity_state* State) {
	if(State->Time < this->GetStart()) {
		this->_source->_add_entity_state(State);
	} else {
		if(State->Time > this->_end) {
			assert(this->_derived.size() < 1); // New state falls off into another derived reality.
			this->_end = State->Time;
		}

		// Actual adding goes here!
		assert(false);

	}
}