/************************************************/
/*	Object Implementation     					*/
/************************************************/
#include "object.h"
#include "world.h"

using namespace nullity;

/****************************************/
/*	Object								*/
/****************************************/
//--
IObject::IObject() {
	this->_entity = NULL;
	this->_frame = NULL;
}

//--
IObject::~IObject() {

}

//--
void IObject::Update(TimeStep Time) {

}

//--
void IObject::Destroy() {
	delete this;
}

//--
Vector IObject::GetPosition() {
	return Vector();
}

//--
Vector IObject::GetRotation() {
	return Vector();
}

//--
IFrame* IObject::GetFrame() {
	return this->_frame;
}

//--
IEntity* IObject::GetEntity() {
	return this->_entity;
}

//--
IVisual* IObject::CreateVisual(VisualParameters Params) {
	return NULL;
}

//--
int IObject::GetSpecialFlag() { 
	return 0x00000000;
}

//--
IObject* IObject::Clone() {
	return NULL;
}

//--
IObject* nullity::Clone(IObject* Object) {
	IObject* clone = Object->Clone();
	if(clone != NULL) {
		clone->_entity = Object->_entity;
	}
	return clone;
}

//--
void nullity::SetObjectEntity(IObject* Object, IEntity* Entity) {
	Object->_entity = Entity;
}

//--
void nullity::SetObjectFrame(IObject* Object, IFrame* Frame) {
	Object->_frame = Frame;
}