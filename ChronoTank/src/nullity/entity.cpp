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
SIMPLE_INTERFACE_CLASS(IEntityInterface)
SIMPLE_INTERFACE_CLASS(IVisualEntity)
SIMPLE_INTERFACE_CLASS(IDynamicEntity)

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

//--
Ptr<Entity> Clone(Ptr<Entity> Entity) {
	return Entity->Clone();
}