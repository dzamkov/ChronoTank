/************************************************/
/*	Interface Implementation					*/
/************************************************/
#include "interface.h"

using namespace nullity;

/****************************************/
/*	Interface   						*/
/****************************************/
//--
Interface::Interface(InterfaceClass* Class, Ptr<IObject> Owner) {
	this->Class = Class;
	this->Owner = Owner;
}

//--
Interface* Interface::GetBase(InterfaceClass* Class) {
	InterfaceClass* cl = this->Class;
	std::map<InterfaceClass*, Interface*> map = cl->GetBases(this);
	std::map<InterfaceClass*, Interface*>::iterator it = map.find(Class);
	if(it != map.end()) {
		// Direct base found.
		return (*it).second;
	} else {
		for(it = map.begin(); it != map.end(); it++) {
			Interface* id = (*it).second;
			Interface* iid = id->GetBase(Class);
			if(iid != NULL) {
				return iid;
			}
		}
		return NULL;
	}
}

//--
std::map<InterfaceClass*, Interface*> InterfaceClass::GetBases(Interface* I) {
	return std::map<InterfaceClass*, Interface*>();
}