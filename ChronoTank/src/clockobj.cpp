/************************************************/
/*	Clock Object Implementation     			*/
/************************************************/
#include <irrlicht.h>
#include <sstream>
#include <iomanip>

#include "clockobj.h"

using namespace nullity;
using namespace ctank;
using namespace irr;

/****************************************/
/*	Clock object						*/
/****************************************/
//--

//--
class ClockObjectDef : public IClockObject {
public:
	void		Init();
	void		Update(TimeStep Time);
	void		Destroy();

	Vector		GetPosition();
	Vector		GetRotation();
	IVisual*	CreateVisual(VisualParameters Params);

	TimeStep	GetTime();
	void		SetTime(TimeStep Time);
	void		SetPosition(Vector Position);

	Vector			Position;
	TimeStep		Time;
	TimeStep		TempTime;
};

//--
class ClockObjectVisual : public IVisual {
public:
	ClockObjectVisual(VisualParameters Params, ClockObjectDef* Obj);

	IObject*	GetTarget();
	void		Update(TimeStep Time);
	void		Render();

	gui::IGUIFont*		Font;
	ClockObjectDef*		Target;
	VisualParameters	Params;
};

//--
IClockObject* ctank::CreateClockObject() {
	ClockObjectDef* cd = new ClockObjectDef();
	cd->Init();
	return cd;
}

//--
void ClockObjectDef::Init() {
	this->Position = Vector(0.0f, 0.0f, 0.0f);
	this->Time = 0.0f;
	this->TempTime = 0.0f;
}

//--
void ClockObjectDef::Update(TimeStep Time) {
	//Using temptime is a trick to increase accuracy.
	float prevtime = this->Time;
	this->Time += Time + this->TempTime;
	this->TempTime -= (this->Time - prevtime) - Time;
}

//--
void ClockObjectDef::Destroy() {
	IObject::Destroy();
}

//--
Vector ClockObjectDef::GetPosition() {
	return this->Position;
}

//--
Vector ClockObjectDef::GetRotation() {
	return Vector();
}

//--
TimeStep ClockObjectDef::GetTime() {
	return this->Time;
}

//--
void ClockObjectDef::SetTime(TimeStep Time) {
	this->Time = Time;
}

//--
void ClockObjectDef::SetPosition(Vector Position) {
	this->Position = Position;
}

//--
IVisual* ClockObjectDef::CreateVisual(VisualParameters Params) {
	return new ClockObjectVisual(Params, this);
}

//--
ClockObjectVisual::ClockObjectVisual(VisualParameters Params, ClockObjectDef* Obj)
: Params(Params)
{
	this->Target = Obj;
	this->Font = this->Params.Device->getGUIEnvironment()->getFont("Font/LucidaConsole10.png");
}

//--
void ClockObjectVisual::Render() {
	scene::ISceneManager* smgr = this->Params.SceneManager;
	video::IVideoDriver* driver = smgr->getVideoDriver();

	// Get screen position
	Vector pos = this->Target->Position;
	core::position2di spos = smgr->getSceneCollisionManager()->getScreenCoordinatesFrom3DPosition(pos);
	core::rect<s32> fpos(spos.X, spos.Y, spos.X, spos.Y);

	// Format time
	TimeStep time = this->Target->Time;
	bool up = time >= 0.0f; time = up ? time : -time;
	int min = (int)(time / 60.0f);
	int sec = (int)fmod(time, 60.0f);
	int mil = (int)(fmod(time, 1.0f) * 1000.0f);
	std::ostringstream oss;
	oss << std::setfill('0');
	if (up) {
		oss << '+';
	} else {
		oss << '-';
	}
	if(min < 1000) {
		oss << std::setw(3) << min;
		oss << ':';
		oss << std::setw(2) << sec;
		oss << ':';
		oss << std::setw(3) << mil;
	} 
	else
	{
		oss << "---:--:---";
	}
	std::string timestringn(oss.str());
	std::wstring timestring(timestringn.length(), L' ');
	std::copy(timestringn.begin(), timestringn.end(), timestring.begin());

	// Draw
	this->Font->draw(timestring.c_str(), fpos, video::SColor(255, 255, 255, 255), true, true);
}

//--
IObject* ClockObjectVisual::GetTarget() {
	return this->Target;
}

//--
void ClockObjectVisual::Update(TimeStep Time) {

}