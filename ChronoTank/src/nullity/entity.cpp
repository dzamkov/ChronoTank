/************************************************/
/*	Entity Implementation		     			*/
/************************************************/
#include <assert.h>

#include "entity.h"

using namespace nullity;

/****************************************/
/*	Entity								*/
/****************************************/
//--
INTERFACE_CLASS(IEntityInterface)

//--
void Entity::Init(IInterface* MainInterface) {
	this->MainInterface = MainInterface;
	this->EntityInterface = (IEntityInterface*)(this->MainInterface->GetBase(IEntityInterface::Class));
	assert(this->EntityInterface != NULL);
}

//--
void Entity::Destroy() {
	delete this->MainInterface;
}

//--
Ptr<Entity> Entity::Clone() {
	return this->EntityInterface->Clone();
}