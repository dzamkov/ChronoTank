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
void MessageEvent::Remove() {
	delete this;
}

//--
void World::Init(IObject* Root) { 
	this->_rootreality = new Reality();
	this->_rootreality->Init(0.0f, this->CreateFrame());
	this->_rootreality->GetWriteFrame()->SpawnObject(Root);
}

//--
void World::Destroy() {
	// Destroy frames
	for(std::vector<Frame>::iterator it = this->_frames.begin();
		it != this->_frames.end(); it++)
	{
		(*it).Destroy();
	}
	delete this->_rootreality;
	delete this;
}

//--
IFrame* World::GetOriginFrame() {
	return this->_rootreality->GetWriteFrame();
}

//--
IReality* World::GetRootReality() {
	return this->_rootreality;
}

//--
Entity* World::CreateEntity() {
	Entity e;
	this->_entities.push_back(e);
	return &(this->_entities.back());
}

//--
Frame*	World::CreateFrame() {
	Frame f;
	f._world = this;
	this->_frames.push_back(f);
	return &(this->_frames.back());
}