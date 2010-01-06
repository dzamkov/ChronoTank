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
Reality::Reality() {
	this->_source.SetOwner(this);
	this->_writeframe.SetOwner(this);
}

//--
Ptr<IReality> Reality::GetSource() {
	return this->_source;
}

//--
TimeStep Reality::GetStart() {
	if(this->_source.IsNull()) {
		// Farest time possible time in the past.
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
void Reality::Init(TimeStep Origin, Ptr<Frame> WriteFrame) {
	this->_source.MakeNull();
	this->_end = Origin;
	this->_writeframe = WriteFrame;
	this->_frames.insert(Ptr<Frame>(this, WriteFrame));
}

// Bunch of crap left unimplemented until it's needed.

//--
void Reality::RecordState(TimeStep Time, Ptr<Entity> Old, Ptr<Entity> New) {
	// Since none of the methods to access this data are implemented, we dont need to
	// do anything.
}

//--
Ptr<Frame> Reality::GetWriteFrame() {
	return this->_writeframe;
}

//--
void Reality::_remove_frame(Ptr<Frame> F) {
	if(this->_writeframe == F) {
		this->_writeframe.MakeNull();
	}
	this->_frames.erase(Ptr<Frame>(this, F));
}