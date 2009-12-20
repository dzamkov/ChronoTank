/************************************************/
/*	Vehicle Implementation     					*/
/************************************************/
#include <irrlicht.h>
#include "vehicle.h"

using namespace nullity;
using namespace ctank;
using namespace irr;

/****************************************/
/*	Player tank							*/
/****************************************/
//--
class PlayerTankDef : public IVehicle {
public:
	void		Init();
	void		Update(TimeStep Time);
	void		Destroy();

	Vector		GetPosition();
	Vector		GetRotation();
	IVisual*	CreateVisual(VisualParameters Params);

	void		SetVehicleControl(VehicleControl Key, float Value);

	Vector		Position;
	float		Yaw;

	float		FowardControl;
	float		TurnControl;
};

//--
IVehicle* ctank::CreatePlayerTank() {
	PlayerTankDef* ptd = new PlayerTankDef();
	ptd->Init();
	return ptd;
}

//--
void PlayerTankDef::Init() {
	this->Position = Vector(0.0f, 0.0f, 0.0f);
	this->Yaw = 0.0f;
	this->FowardControl = 0.0f;
	this->TurnControl = 0.0f;
}

//--
void PlayerTankDef::Update(TimeStep Time) {
	float fspeed = core::clamp(this->FowardControl, -0.5f, 1.0f) * 3.0f;
	float tspeed = core::clamp(this->TurnControl, -1.0f, 1.0f) * 90.0f;

	this->Yaw += tspeed * Time;
	float deg = core::degToRad(this->Yaw);
	this->Position += Time * Vector(sin(deg), 0.0f, cos(deg)) * fspeed;
}

//--
void PlayerTankDef::Destroy() {
	delete this;
}

//--
Vector PlayerTankDef::GetPosition() {
	return this->Position;
}

//--
Vector PlayerTankDef::GetRotation() {
	return Vector(0.0f, this->Yaw, 0.0f);
}

//--
IVisual* PlayerTankDef::CreateVisual(VisualParameters Params) {
	io::IReadFile* vissrc = Params.Device->getFileSystem()->createAndOpenFile("ChronoTank/Basic/Placeholder.3ds");
	IVisual* vis = LoadVisualFromFile(Params, vissrc, this);
	vissrc->drop();
	return vis;
}

//--
void PlayerTankDef::SetVehicleControl(VehicleControl Key, float Value) {
	switch(Key) {
		case VehicleControlFoward: this->FowardControl = Value; break;
		case VehicleControlTurn: this->TurnControl = Value; break;
	}
}