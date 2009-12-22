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