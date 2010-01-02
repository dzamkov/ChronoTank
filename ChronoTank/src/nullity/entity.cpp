/************************************************/
/*	Entity Implementation		     			*/
/************************************************/
#include "entity.h"

using namespace nullity;

/****************************************/
/*	Entity								*/
/****************************************/
//--
StackPtr<IEntity> EntityClass::Create() {
	return NULL;
}

//--
std::string EntityClass::GetName() {
	return "";
}

//--
StackPtr<IEntity> nullity::Clone(StackPtr<IEntity>::Ref Entity) {
	StackPtr<IEntity> ent = Entity->GetClass()->Create();
	Entity->Clone(ent);
	return ent;
}