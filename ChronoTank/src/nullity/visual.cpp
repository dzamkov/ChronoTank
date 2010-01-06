/************************************************/
/*	Visual Implementation     					*/
/************************************************/
#include <irrlicht.h>

#include "visual.h"
#include "object.h"

using namespace nullity;
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
void IVisual::Update(Ptr<Entity> Entity) {

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
	LoadedVisual(VisualParameters Params, io::IReadFile* File);

	void	Update(Ptr<Entity> Entity);
	void	Destroy();

	scene::ISceneNode*		Node;
};

//--
IVisual* nullity::LoadVisualFromFile(VisualParameters Params, io::IReadFile* File) {
	return new LoadedVisual(Params, File);
}

//--
LoadedVisual::LoadedVisual(
	VisualParameters Params, 
	io::IReadFile* File)
{
	scene::IMesh* mesh = Params.SceneManager->getMesh(File);
	this->Node = Params.SceneManager->addMeshSceneNode(mesh, Params.ParentNode, Params.ID);
}

//--
void LoadedVisual::Destroy() {
	this->Node->remove();
	IVisual::Destroy();
}

//--
void LoadedVisual::Update(Ptr<Entity> Entity) {

}