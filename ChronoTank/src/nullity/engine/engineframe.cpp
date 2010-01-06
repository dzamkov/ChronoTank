/************************************************/
/*	Frame Implementation     					*/
/************************************************/
#include "engineframe.h"
#include "engineworld.h"

using namespace nullity;

/************************************************/
/*	Frame functions definitions     			*/
/************************************************/
//--
EntityEx::EntityEx() {
	this->Entity.SetOwner(this);
	this->Visual.SetOwner(this);
}

//--
void EntityEx::Destroy() {
	
}

//--
void EntityEx::Manage(int Flags, const VisualParameters& Params) {
	if(!this->Visual.IsNull()) {
		if(Flags & VisualFlagFullDelete) {
			this->Visual.MakeNull();
		}
	} else {
		if(Flags & VisualFlagAllowCreation) {
			this->Visual = this->Entity->CreateVisual(Params);
		}
	}
}

//--
void EntityEx::Update(int Flags) {
	if(this->Visual != NULL) {
		this->Visual->Update(this->Entity);
	}
}

//--
Frame::Frame()
{
	this->_reality.SetOwner(this);
	this->_world.SetOwner(this);
}

//--
void Frame::Init(TimeStep Time, Ptr<Reality> Reality, bool Write) {
	this->_time = Time;
	this->_reality = Reality;
	this->_write = Write;
	this->SetVisualFlags(VisualFlagNoCreation | VisualFlagNoRender);
}

//--
void Frame::Destroy() {
	this->_reality->_remove_frame(this);
}

//--
void Frame::Update(TimeStep Time) {
	// Timing
	TimeStep ustart = this->_time;
	TimeStep uend = ustart + Time;

	// Tasks
	this->_tasks.PerformTasks(this);

	// Object updates
	for(_entitymap::iterator it = this->_entities.begin();
		it != this->_entities.end(); it++)
	{
		EntityEx* ent = (*it).second;
		TimeStep utime = Time;
		
		Ptr<Entity> oldent = ent->Entity;
		Ptr<Entity> newent = Clone(oldent);
		newent->Update(utime);
		ent->Entity = newent;
		this->_reality->RecordState(uend, oldent, newent);

		ent->Manage(this->_visflags, this->_visparams);
		ent->Update(this->_visflags);
	}

	// Status
	this->_time = uend;
}

//--
void Frame::SpawnEntity(Ptr<Entity> Entity) {
	this->_tasks.SpawnedEntities.push_back(Ptr<Entity>(this, Entity));
}

//--
void Frame::AddEntity(Ptr<Entity> Entity) {
	Ptr<EntityEx> entex = new EntityEx(); 
	entex->Entity = Entity;
	entex->Manage(this->_visflags, this->_visparams);
	this->_entities[Ptr<Entity>(this, Entity)] = Ptr<EntityEx>(this, entex);
}

//--
void Frame::SetVisualFlags(int Flags) {
	this->_visflags = Flags;
}

//--
void Frame::SetVisualParameters(VisualParameters Params) {
	this->_visparams = Params;
}

//--
void Frame::RenderVisuals() {
	if(this->_visflags & VisualFlagPerformRender) {
		for(_entitymap::iterator it = this->_entities.begin();
			it != this->_entities.end(); it++)
		{
			Ptr<EntityEx> ent = (*it).second;
			if(!ent->Visual.IsNull()) {
				ent->Visual->Render();
			}
		}
	}
}

//--
void Frame::OnRealityDestroyed() {
	this->Destroy();
}

//--
void Frame::_swap_reality(Ptr<Reality> Reality) {
	this->_reality = Reality;
}

//--
void Frame::_tasklist::PerformTasks(Frame* Frame) {
	// Spawning
	for(std::vector<Ptr<Entity>>::iterator it = this->SpawnedEntities.begin();
		it != this->SpawnedEntities.end(); it++)
	{
		Frame->AddEntity(*it);
	}
}