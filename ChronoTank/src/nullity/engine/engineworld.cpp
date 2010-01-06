/************************************************/
/*	World Implementation     					*/
/************************************************/
#include <limits>

#include "engineworld.h"

using namespace nullity;

/************************************************/
/*	World functions definitions     			*/
/************************************************/
//--
World::World() {
	this->_rootreality.SetOwner(this);
}

//--
void World::Init(Ptr<Entity> Root) { 
	this->_rootreality = new Reality();
	this->_rootreality->Init(0.0f, new Frame());
	this->_rootreality->GetWriteFrame()->SpawnEntity(Root);
}

//--
Ptr<IFrame> World::GetOriginFrame() {
	return this->_rootreality->GetWriteFrame();
}

//--
Ptr<IReality> World::GetRootReality() {
	return this->_rootreality;
}