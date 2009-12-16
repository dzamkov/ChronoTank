/************************************************/
/*	Object Implementation     					*/
/************************************************/
#include "object.h"
#include "world.h"

using namespace ctank;

/****************************************/
/*	Object								*/
/****************************************/
//--
IObject::IObject() {
	this->_entity = NULL;
	this->_frame = NULL;
}

//--
void IObject::Update(TimeStep Time) {

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
void IObject::SendMessage(IEntity* Entity, TimeStep Destination, IMessage* Message) {
	this->GetFrame()->SendMessage(this, Entity, Destination, Message);
}

//--
void IObject::SendMessage(TimeStep Destination, IMessage* Message) {
	this->SendMessage(this->GetEntity(), Destination, Message);
}

//--
void IObject::SendMessage(IMessage* Message) {
	this->SendMessage(0.0, Message);
}

//--
void IObject::OnReceiveMessage(IMessageNotice* Notice) {

}

//--
void IObject::OnSkipMessage(IMessageNotice* Notice) {

}

//--
IObject* ctank::Clone(IObject* Object) {
	IObject* clone = Object->Clone();
	if(clone != NULL) {
		clone->_entity = Object->_entity;
	}
	return clone;
}

//--
void ctank::SetObjectEntity(IObject* Object, IEntity* Entity) {
	Object->_entity = Entity;
}

//--
void ctank::SetObjectFrame(IObject* Object, IFrame* Frame) {
	Object->_frame = Frame;
}