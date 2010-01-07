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
	this->Ent.SetOwner(this);
	this->Visual.SetOwner(this);
}

//--
void EntityEx::SetEntity(Ptr<Entity> E) {
	this->Ent = E;
	IInterface* main = this->Ent->MainInterface;
	this->DynamicInt = (IDynamicEntity*)main->GetBase(IDynamicEntity::Class);
	this->VisualInt = (IVisualEntity*)main->GetBase(IVisualEntity::Class);
}

//--
void EntityEx::Destroy() {
	
}

//--
void EntityEx::Manage(int Flags, const VisualParameters& Params) {
	if(this->VisualInt != NULL) {
		if(!this->Visual.IsNull()) {
			if(Flags & VisualFlagFullDelete) {
				this->Visual.MakeNull();
			}
		} else {
			if(Flags & VisualFlagAllowCreation) {
				this->Visual = this->VisualInt->CreateVisual(Params);
			}
		}
	}
}

//--
void EntityEx::Update(int Flags) {
	if(!this->Visual.IsNull()) {
		this->Visual->Update(this->Ent->MainInterface);
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
		
		Ptr<Entity> oldent = ent->Ent;
		Ptr<Entity> newent = Clone(oldent);
		ent->SetEntity(newent);
		ent->DynamicInt->Update(utime);
		this->_reality->RecordState(uend, oldent, newent);

		ent->Manage(this->_visflags, this->_visparams);
		ent->Update(this->_visflags);
	}

	// Status
	this->_time = uend;
}

//--
void Frame::SpawnEntity(Ptr<Entity> E) {
	this->_tasks.SpawnedEntities.push_back(Ptr<Entity>(this, E));
}

//--
void Frame::AddEntity(Ptr<Entity> E) {
	Ptr<EntityEx> entex = new EntityEx(); 
	entex->SetEntity(E);
	entex->Manage(this->_visflags, this->_visparams);
	this->_entities[Ptr<Entity>(this, E)] = Ptr<EntityEx>(this, entex);
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