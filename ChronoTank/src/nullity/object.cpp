/************************************************/
/*	Object Implementation     					*/
/************************************************/
#include <assert.h>

#include "object.h"

using namespace nullity;

/****************************************/
/*	Object								*/
/****************************************/
//--
IObject::IObject() {
	this->_flags = _flag_none;
}

//--
IObject::~IObject() {
	this->_flags = this->_flags | _flag_deallocated;
}

//--
void IObject::Destroy() {
	this->_flags = this->_flags | _flag_destroyed;
	this->Deallocate();
}

//--
void IObject::Deallocate() {
	delete this;
}

//--
void IObject::_incref(IObject* From, int Amount) {
	// From Refs
	if(From != NULL) {
		std::map<IObject*, int>::iterator it = From->_refs_to.find(this);
		if(it == From->_refs_to.end()) {
			From->_refs_to[this] = Amount;
		} else {
			int& rc = (*it).second;
			rc += Amount;
			if(rc == 0) {
				From->_refs_to.erase(it);
			}
		}
	}

	// Refs
	this->_refs += Amount;
	assert(this->_refs >= 0);
	if(this->_refs == 0) {
		this->Destroy();
	}
}