/************************************************/
/*	Interface Implementation					*/
/************************************************/
#include "interface.h"

using namespace nullity;

/****************************************/
/*	Interface   						*/
/****************************************/
//--
IInterface::IInterface(IObject* Owner) {
	this->_owner = Owner;
}

//--
IInterface* IInterface::GetBase(InterfaceClass* Class) {
	std::map<InterfaceClass*, IInterface*> map = this->GetClass()->GetBases(this);
	std::map<InterfaceClass*, IInterface*>::iterator it = map.find(Class);
	if(it != map.end()) {
		// Direct base found.
		return (*it).second;
	} else {
		for(it = map.begin(); it != map.end(); it++) {
			IInterface* id = (*it).second;
			IInterface* iid = id->GetBase(Class);
			if(iid != NULL) {
				return iid;
			}
		}
		return NULL;
	}
}

//--
IObject* IInterface::GetOwner() {
	return this->_owner;
}

//--
std::map<InterfaceClass*, IInterface*> InterfaceClass::GetBases(IInterface* Interface) {
	return std::map<InterfaceClass*, IInterface*>();
}