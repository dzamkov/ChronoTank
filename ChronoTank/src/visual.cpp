/************************************************/
/*	Visual Implementation     					*/
/************************************************/
#include <irrlicht.h>

#include "visual.h"
#include "object.h"

using namespace ctank;
using namespace irr;

/****************************************/
/*	Visual Parameters					*/
/****************************************/
//--
VisualParameters VisualParameters::AsRootNode(scene::ISceneManager* Manager) {
	VisualParameters vp;
	vp.SceneManager = Manager;
	vp.ParentNode = vp.SceneManager->getRootSceneNode();
	vp.Device = NULL;
	vp.ID = -1;
	return vp;
}

//--
VisualParameters VisualParameters::AsRootNode(irr::IrrlichtDevice* Device) {
	VisualParameters vp = VisualParameters::AsRootNode(Device->getSceneManager());
	vp.Device = Device;
	return vp;
}

/****************************************/
/*	Visual								*/
/****************************************/
//--
void IVisual::Destroy() {
	delete this;
}

//--
void IVisual::Render() {

}

/****************************************/
/*	Loaded Visuals						*/
/****************************************/
//--
class LoadedVisual : public IVisual {
public:
	LoadedVisual(VisualParameters Params, io::IReadFile* File, IObject* Object);
	~LoadedVisual();

	void		Destroy();

	void		Update(TimeStep Time);
	IObject*	GetTarget();

	IObject*				Target;
	scene::ISceneNode*		Node;
};

//--
class VisualSerializer : public scene::ISceneUserDataSerializer {
public:
	io::IAttributes*		createUserData(scene::ISceneNode* For);
	void					OnCreateNode(scene::ISceneNode* Node);
	void					OnReadUserData(scene::ISceneNode* For, io::IAttributes* UserData);

	scene::ISceneNode*		PrevRoot;
	scene::ISceneNode*		TargetRoot;
	scene::ISceneNode*		SceneRoot;
};

//--
IVisual* ctank::LoadVisualFromFile(VisualParameters Params, io::IReadFile* File, IObject* Object) {
	return new LoadedVisual(Params, File, Object);
}

//--
LoadedVisual::LoadedVisual(
	VisualParameters Params, 
	io::IReadFile* File, 
	IObject* Object)
{
	this->Target = Object;

	scene::IMesh* mesh = Params.SceneManager->getMesh(File);
	this->Node = Params.SceneManager->addMeshSceneNode(mesh, Params.ParentNode, Params.ID);
}

//--
LoadedVisual::~LoadedVisual() {
}

//--
void LoadedVisual::Destroy() {
	delete this;
}

//--
void LoadedVisual::Update(TimeStep Time) {
	this->Node->setPosition(this->Target->GetPosition());
	this->Node->setRotation(this->Target->GetRotation());
}

//--
IObject* LoadedVisual::GetTarget() {
	return this->Target;
}