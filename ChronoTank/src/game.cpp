/************************************************/
/*	Game Implementation     					*/
/************************************************/
#include <irrlicht.h>
#include <math.h>
#include <string>

#include "nullity/visual.h"
#include "nullity/world.h"

#include "game.h"

#include "entities/unitent.h"
#include "entities/clockent.h"

using namespace nullity;
using namespace ctank;
using namespace irr;

/****************************************/
/*	Game Entity							*/
/****************************************/
//--
class GameEntity : public Interface,
	public IEntityInterface,
	public IDynamicEntity {
public:
	DECLARE_INTERFACE_CLASS(GameEntity)

	void			FrameInteract(Ptr<IFrame> Frame);
	Ptr<Entity>		Clone();

	bool			ObjsCreated;
	Ptr<Entity>		Tank;
};

//--
BEGIN_INTERFACE_CLASS(GameEntity)
	INTERFACE_CLASS_NAME(GameEntity)
	BEGIN_INTERFACE_CLASS_BASES(GameEntity)
		INTERFACE_CLASS_BASE(IEntityInterface)
		INTERFACE_CLASS_BASE(IDynamicEntity)
	END_INTERFACE_CLASS_BASES
END_INTERFACE_CLASS(GameEntity)

//--
GameEntity::GameEntity(Ptr<IObject> Owner) :
IEntityInterface(Owner),
IDynamicEntity(Owner),
Interface(GameEntity::Class, Owner)
{
	this->ObjsCreated = false;
	this->Tank.SetOwner(Owner);
}

//--
void GameEntity::FrameInteract(Ptr<IFrame> Frame) {
	if(!this->ObjsCreated) {
		this->ObjsCreated = true;

		// Player tank
		this->Tank = CreatePlayerTank();
		Frame->SpawnEntity(this->Tank);

		// Clocks
		Ptr<Entity> clock = new Entity();
		clock->Init(new ClockEntity(Ptr<IObject>(clock)));
		Frame->SpawnEntity(clock);
	}
}

//--
Ptr<Entity> GameEntity::Clone() {
	Ptr<Entity> e = new Entity();
	GameEntity* ge = new GameEntity(Ptr<IObject>(e));
	e->Init(ge);
	ge->ObjsCreated = this->ObjsCreated;
	ge->Tank = this->Tank;
	return e;
}

/****************************************/
/*	Custom game							*/
/****************************************/
//--
class GameDef : public IGame {
public:
	void		Init(IrrlichtDevice* Device);
	void		Update(TimeStep Time);
	void		Render();
	void		Destroy();

	bool		OnEvent(const SEvent& Event);

private:
	IrrlichtDevice*				_device;
	video::IVideoDriver*		_driver;
	scene::ISceneManager*		_smgr;
	scene::ICameraSceneNode*	_camera;
	io::IFileSystem*			_fsys;

	std::string		_maindir;

	Ptr<IWorld>			_world;
	Ptr<IFrame>			_frame;

	TimeStep			_timerate;
	TimeStep			_curtimerate;
};

//--
IGame* ctank::CreateGame() {
	return new GameDef();
}

//--
void GameDef::Init(irr::IrrlichtDevice* Device) {
	this->_device = Device;
	this->_driver = this->_device->getVideoDriver();
	this->_smgr = this->_device->getSceneManager();
	this->_fsys = this->_device->getFileSystem();

	this->_maindir = std::string(this->_fsys->getWorkingDirectory().c_str()) + "/..";
	this->_fsys->changeWorkingDirectoryTo(this->_maindir.c_str());
	this->_maindir = this->_fsys->getWorkingDirectory().c_str();
	this->_fsys->changeWorkingDirectoryTo((this->_maindir + "/Media").c_str());

	this->_camera = this->_smgr->addCameraSceneNode();
	this->_camera->setPosition(core::vector3df(0.0f, 10.0f, -5.0f));
	this->_camera->setTarget(core::vector3df(0.0f, 0.0f, 0.0f));

	this->_smgr->loadScene("Default.irr");

	// World
	Ptr<Entity> ge = new Entity();
	ge->Init(new GameEntity(Ptr<IObject>(ge)));
	this->_world = CreateWorld();
	this->_world->Init(ge);
	this->_frame = this->_world->GetOriginFrame();

	// Visuals for frame
	this->_frame->SetVisualParameters(VisualParameters::AsRootNode(this->_device));
	this->_frame->SetVisualFlags(VisualFlagAllowCreation | VisualFlagPerformRender);

	// Time rate
	this->_timerate = 1.0f;
	this->_curtimerate = 1.0f;
}

//--
void GameDef::Update(TimeStep Time) {
	TimeStep crate = 2.0f * Time;
	TimeStep trl = log(this->_timerate);
	TimeStep ctrl = log(this->_curtimerate);
	if(ctrl > trl) {
		ctrl -= crate;
		ctrl = ctrl < trl ? trl : ctrl;
	} else {
		ctrl += crate;
		ctrl = ctrl > trl ? trl : ctrl;
	}
	this->_curtimerate = exp(ctrl);

	TimeStep tchange = Time * this->_curtimerate;
	this->_frame->Update(tchange);
}

//--
void GameDef::Render() {
	this->_driver->beginScene(true, true, video::SColor(255, 104, 119, 126));
	this->_smgr->drawAll();
	this->_frame->RenderVisuals();
	this->_driver->endScene();
}

//--
void GameDef::Destroy() {
}

//--
bool GameDef::OnEvent(const SEvent& Event) {
	if(Event.EventType == EET_KEY_INPUT_EVENT) {
		const SEvent::SKeyInput& kevent = Event.KeyInput;

		if(kevent.Key == KEY_KEY_Q && kevent.PressedDown) this->_timerate = 3.0f;
		if(kevent.Key == KEY_KEY_Q && !kevent.PressedDown) this->_timerate = 1.0f;
		if(kevent.Key == KEY_KEY_E && kevent.PressedDown) this->_timerate = 1.0f / 3.0f;
		if(kevent.Key == KEY_KEY_E && !kevent.PressedDown) this->_timerate = 1.0f;
	}
	return false;
}