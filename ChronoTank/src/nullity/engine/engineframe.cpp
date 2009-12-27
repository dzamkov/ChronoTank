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
void ObjectEx::Destroy() {
	this->Object->Destroy();
	if(this->Visual != NULL) {
		this->Visual->Destroy();
	}
}

//--
void ObjectEx::Manage(int Flags, const VisualParameters& Params) {
	if(this->Visual == NULL) {
		if(Flags & VisualFlagFullDelete) {
			this->Visual->Destroy();
			this->Visual = NULL;
		}
	} else {
		if(Flags & VisualFlagAllowCreation) {
			this->Visual = this->Object->CreateVisual(Params);
		}
	}
}

//--
void ObjectEx::Update(TimeStep Time, int Flags) {
	this->Object->Update(Time);
	if(this->Visual != NULL) {
		this->Visual->Update(Time);
	}
}

//--
void Frame::Init(TimeStep Time, Reality* Reality, bool Write) {
	this->_time = Time;
	this->_reality = Reality;
	this->_write = Write;
	this->SetVisualFlags(VisualFlagNoCreation | VisualFlagNoRender);
}

//--
void Frame::Destroy() {
	for(std::map<Entity*, ObjectEx>::iterator it = this->_objects.begin();
		it != this->_objects.end(); it++)
	{
		(*it).second.Destroy();
	}
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
	for(std::map<Entity*, ObjectEx>::iterator it = this->_objects.begin();
		it != this->_objects.end(); it++)
	{
		ObjectEx& obj = (*it).second;
		TimeStep utime = Time;
		obj.Manage(this->_visflags, this->_visparams);
		obj.Update(utime, this->_visflags);
		this->_reality->RecordState(uend, obj.Object);
	}

	// Status
	this->_time = uend;
}

//--
void Frame::SpawnObject(IObject* Object) {
	SetObjectEntity(Object, this->_world->CreateEntity());
	this->_tasks.SpawnedObjects.push_back(Object);
}

//--
void Frame::AddObject(IObject* Object) {
	ObjectEx objex; objex.Object = Object;
	SetObjectFrame(objex.Object, this);
	objex.Manage(this->_visflags, this->_visparams);
	this->_objects[(Entity*)(objex.Object->GetEntity())] = objex;
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
		for(std::map<Entity*, ObjectEx>::iterator it = this->_objects.begin();
			it != this->_objects.end(); it++)
		{
			ObjectEx& obj = (*it).second;
			if(obj.Visual != NULL) {
				obj.Visual->Render();
			}
		}
	}
}

//--
void Frame::OnRealityDestroyed() {
	this->Destroy();
}

//--
IObject* Frame::ObjectFor(IEntity* E) {
	std::map<Entity*, ObjectEx>::iterator ent = this->_objects.find((Entity*)E);
	if(ent != this->_objects.end()) {
		return (*ent).second.Object;
	} else {
		return NULL;
	}
}

//--
void Frame::_swap_reality(Reality* Reality) {
	this->_reality = Reality;
}

//--
void Frame::_tasklist::PerformTasks(Frame* Frame) {
	// Spawning
	for(std::vector<IObject*>::iterator oit = this->SpawnedObjects.begin();
		oit != this->SpawnedObjects.end(); oit++)
	{
		Frame->AddObject(*oit);
	}
}