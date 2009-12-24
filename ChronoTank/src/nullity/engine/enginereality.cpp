/************************************************/
/*	Reality Implementation     					*/
/************************************************/
#include <assert.h>
#include <limits>
#include <map>
#include <set>
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
	return this->_end;
}

//--
void Reality::Init() {
	this->_source = NULL;
	this->_end = -std::numeric_limits<TimeStep>::infinity();
}

//--
void Reality::Destroy() {
	for(std::set<Reality*>::iterator it = this->_derived.begin();
		it != this->_derived.end(); it++)
	{
		Reality* d = *it;
		d->_source = NULL;
		d->Destroy();
	}
	if(this->_source != NULL) {
		this->_source->_derived.erase(this);
	}
	delete this;
}

//--
Reality* Reality::Split() {
	Reality* nr = new Reality();
	nr->_source = this;
	nr->_end = this->_end;
	this->_derived.insert(nr);
	return nr;
}

//--
Reality* Reality::Cut(TimeStep Time) {
	// Check for valid request
	assert(Time > this->GetStart() && Time <= this->GetEnd());

	// Meta-data
	Reality* nr = new Reality();
	nr->_source = this;
	nr->_end = this->_end;
	this->_derived.insert(nr);

	// Entity information
	for(_datatype::iterator it = this->_data.begin();
		it != this->_data.end(); it++)
	{
		IEntity* e = (*it).first;
		_histtype& h = (*it).second;
		std::vector<_histtype::iterator> inforemove;
		for(_histtype::iterator hit = h.begin();
			hit != h.end(); hit++)
		{
			_info& i = *hit;
			if(i.Time >= Time) {
				nr->_data[e].push_back(i);
				inforemove.push_back(hit);
			}
		}
		for(std::vector<_histtype::iterator>::iterator hiit = inforemove.begin();
			hiit != inforemove.end(); hiit++)
		{
			h.erase(*hiit);
		}
	}

	return nr;
}

//--
void Reality::Merge() {
	assert(this->_derived.size() == 1);

	// Meta-data
	Reality* r = *(this->_derived.begin());
	this->_derived.empty();
	this->_end = r->_end;

	// Entity information
	for(_datatype::iterator it = r->_data.begin();
		it != r->_data.end(); it++)
	{
		IEntity* e = (*it).first;
		_histtype& h = (*it).second;
		for(_histtype::iterator hit = h.begin();
			hit != h.end(); hit++)
		{
			_info& i = *hit;
			this->_data[e].push_back(i);
		}
	}

	// Reality relation information
	for(std::set<Reality*>::iterator dit = r->_derived.begin();
		dit != r->_derived.end(); dit++)
	{
		Reality* d = *dit;
		d->_source = this;
		this->_derived.insert(d);
	}

	delete r;
}