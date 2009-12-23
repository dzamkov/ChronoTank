/************************************************/
/*	Reality Implementation     					*/
/************************************************/
#include <assert.h>
#include <limits>
#include <stack>

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
		return this->_source->GetEnd();
	}
}

//--
TimeStep Reality::GetEnd() {
	return this->_timeblock.End;
}

//--
void Reality::Init() {
	this->_source = NULL;
	this->_timeblock.End = -std::numeric_limits<TimeStep>::infinity();
	this->_timeblock.Prev = NULL;
	this->_timeblock.Child = NULL;
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
	nr->_timeblock.End = this->GetEnd();
	nr->_timeblock.Prev = &this->_timeblock;
	nr->_timeblock.Child = NULL;
	nr->_source = this;
	this->_derived.push_back(nr);
	return nr;
}

//--
void Reality::RecordState(TimeStep Time, IObject* Object) {

}

//--
void Reality::RecordRemoval(TimeStep Time, IObject* Object) {
	
}