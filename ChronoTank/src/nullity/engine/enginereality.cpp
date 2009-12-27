/************************************************/
/*	Reality Implementation     					*/
/************************************************/
#include <assert.h>
#include <limits>
#include <map>
#include <set>
#include <stack>

#include "engineframe.h"
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
void Reality::Init(TimeStep Origin, Frame* WriteFrame) {
	this->_source = NULL;
	this->_end = Origin;

	WriteFrame->Init(Origin, this, true);
	this->_writeframe = WriteFrame;
	this->_frames.insert(WriteFrame);
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
	for(std::set<Frame*>::iterator fit = this->_frames.begin();
		fit != this->_frames.end(); fit++)
	{
		(*fit)->OnRealityDestroyed();
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

	if(this->_writeframe != NULL) {
		this->_writeframe->_swap_reality(nr);
		this->_frames.erase(this->_writeframe);
		nr->_frames.insert(this->_writeframe);
		nr->_writeframe = this->_writeframe;
		this->_writeframe = NULL;
	}

	return nr;
}

//--
Reality* Reality::Cut(TimeStep Time) {
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

	// Frames
	std::vector<std::set<Frame*>::iterator> frameremove;
	for(std::set<Frame*>::iterator fit = this->_frames.begin();
		fit != this->_frames.end(); fit++)
	{
		Frame* f = *fit;
		if(this->_writeframe == f || f->_time > Time) {
			if(this->_writeframe == f) {
				nr->_writeframe = f;
				this->_writeframe = NULL;
			}
			f->_swap_reality(nr);
			frameremove.push_back(fit);
			nr->_frames.insert(f);
		}
	}
	for(std::vector<std::set<Frame*>::iterator>::iterator fiit = frameremove.begin();
		fiit != frameremove.end(); fiit++)
	{
		this->_frames.erase(*fiit);
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

	// Frames
	for(std::set<Frame*>::iterator fit = r->_frames.begin();
		fit != r->_frames.end(); fit++)
	{
		Frame* f = *fit;
		if(r->_writeframe == f) {
			assert(this->_writeframe == NULL);
			this->_writeframe = f;
		}
		f->_swap_reality(this);
		this->_frames.insert(f);
	}

	delete r;
}

//--
void Reality::RecordState(TimeStep Time, IObject* Object) {
	assert(this->_writeframe != NULL);

	// Object information
	IEntity* ent = Object->GetEntity();
	IObject* sta = Clone(Object);
	_info i; i.Time = Time; i.State = sta;

	// Find insertion location
	_histtype& h = this->_data[ent];
	_histtype::iterator it = h.begin();
	_histtype::iterator nit = it;
	while(nit != h.end() && (*nit).Time < Time) {
		it = nit;
		nit++;
	}

	// Insert
	h.insert(it, i);
}

//--
Frame* Reality::GetWriteFrame() {
	return this->_writeframe;
}

//--
void Reality::_remove_frame(Frame* Frame) {
	this->_frames.erase(Frame);
	if(this->_writeframe == Frame) {
		this->_writeframe = NULL;
	}
}