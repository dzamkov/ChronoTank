/************************************************/
/*	Game Implementation     					*/
/************************************************/
#include <irrlicht.h>
#include <math.h>
#include <string>

#include "game.h"
#include "visual.h"
#include "world.h"
#include "vehicle.h"
#include "clockobj.h"
#include "timemanipulator.h"

using namespace ctank;
using namespace irr;

/****************************************/
/*	Game object							*/
/****************************************/
//--
class GameObj : public IObject {
public:
	void	Init();
	void	Update(TimeStep Time);
	void	Destroy();

	IObject*	Clone();

	bool		ObjsCreated;
	IVehicle*	Tank;
};

//--
void GameObj::Init() {
	this->ObjsCreated = false;
	this->Tank = NULL;
}

//--
void GameObj::Update(TimeStep Time) {
	if(!this->ObjsCreated) {
		this->ObjsCreated = true;
		IFrame* frame = this->GetFrame();

		// Player tank
		this->Tank = CreatePlayerTank();
		frame->SpawnObject(this->Tank);

		// Clocks
		IClockObject* clock;
		clock = CreateClockObject();
		clock->SetPosition(Vector(-2.0f, 0.0f, 3.5f));
		frame->SpawnObject(clock);

		clock = CreateClockObject();
		clock->SetPosition(Vector(2.0f, 0.0f, -3.5f));
		frame->SpawnObject(clock);
	}
}

//--
void GameObj::Destroy() {
	delete this;
}

//--
IObject* GameObj::Clone() {
	GameObj* go = new GameObj();
	go->ObjsCreated = this->ObjsCreated;
	go->Tank = this->Tank;
	return go;
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

	IWorld*			_world;
	IFrame*			_frame;

	GameObj*		_gameobj;

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

	this->_maindir = this->_fsys->getWorkingDirectory(); this->_maindir = this->_maindir + "/..";
	this->_fsys->changeWorkingDirectoryTo(this->_maindir.c_str());
	this->_maindir = this->_fsys->getWorkingDirectory();
	this->_fsys->changeWorkingDirectoryTo((this->_maindir + "/Media").c_str());

	this->_camera = this->_smgr->addCameraSceneNode();
	this->_camera->setPosition(core::vector3df(0.0f, 10.0f, -5.0f));
	this->_camera->setTarget(core::vector3df(0.0f, 0.0f, 0.0f));

	this->_smgr->loadScene("Default.irr");

	// World
	this->_gameobj = new GameObj();
	this->_gameobj->Init();
	this->_world = CreateWorld();
	this->_world->Init(this->_gameobj);
	this->_frame = this->_world->CreateOriginFrame();

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
	if(this->_gameobj->Tank == NULL) {
		this->_frame->Update(tchange);
	} else {
		this->_frame->UpdateForObject(this->_gameobj->Tank, tchange);
	}
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
	this->_frame->Destroy();
	this->_world->Destroy();
}

//--
bool GameDef::OnEvent(const SEvent& Event) {
	if(Event.EventType == EET_KEY_INPUT_EVENT) {
		const SEvent::SKeyInput& kevent = Event.KeyInput;
		IVehicle* tank = this->_gameobj->Tank;
		if(tank != NULL) {
			if(kevent.Key == KEY_KEY_W && kevent.PressedDown) tank->SetVehicleControl(VehicleControlFoward, 1.0f);
			if(kevent.Key == KEY_KEY_W && !kevent.PressedDown) tank->SetVehicleControl(VehicleControlFoward, 0.0f);
			if(kevent.Key == KEY_KEY_S && kevent.PressedDown) tank->SetVehicleControl(VehicleControlFoward, -1.0f);
			if(kevent.Key == KEY_KEY_S && !kevent.PressedDown) tank->SetVehicleControl(VehicleControlFoward, 0.0f);
			if(kevent.Key == KEY_KEY_A && kevent.PressedDown) tank->SetVehicleControl(VehicleControlTurn, -1.0f);
			if(kevent.Key == KEY_KEY_A && !kevent.PressedDown) tank->SetVehicleControl(VehicleControlTurn, 0.0f);
			if(kevent.Key == KEY_KEY_D && kevent.PressedDown) tank->SetVehicleControl(VehicleControlTurn, 1.0f);
			if(kevent.Key == KEY_KEY_D && !kevent.PressedDown) tank->SetVehicleControl(VehicleControlTurn, 0.0f);
		}

		if(kevent.Key == KEY_KEY_Q && kevent.PressedDown) this->_timerate = 3.0f;
		if(kevent.Key == KEY_KEY_Q && !kevent.PressedDown) this->_timerate = 1.0f;
		if(kevent.Key == KEY_KEY_E && kevent.PressedDown) this->_timerate = 1.0f / 3.0f;
		if(kevent.Key == KEY_KEY_E && !kevent.PressedDown) this->_timerate = 1.0f;
	}
	return false;
}