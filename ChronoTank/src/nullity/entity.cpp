/************************************************/
/*	Entity Implementation		     			*/
/************************************************/
#include "entity.h"

using namespace nullity;

/****************************************/
/*	Entity								*/
/****************************************/
//--
Ptr<IEntity> EntityClass::Create() {
	return NULL;
}

//--
std::string EntityClass::GetName() {
	return "";
}

//--
Ptr<IEntity> nullity::Clone(Ptr<IEntity> Entity) {
	Ptr<IEntity> ent = Entity->GetClass()->Create();
	Entity->Clone(ent);
	return ent;
}