/************************************************/
/*	Clock Entity Implementation     			*/
/************************************************/
#include <irrlicht.h>
#include <sstream>
#include <iomanip>

#include "../nullity/visual.h"

#include "clockent.h"

using namespace nullity;
using namespace ctank;
using namespace irr;

/****************************************/
/*	Clock Entity						*/
/****************************************/
//--
class ClockEntityVisual : public IVisual {
public:
	ClockEntityVisual(VisualParameters Params);

	void	Update(Ptr<IEntity> Entity);
	void	Render();

private:
	Ptr<ClockEntity>	_target;
	VisualParameters	_params;
	gui::IGUIFont*		_font;
};

//--
struct : public EntityClass {
	std::string GetName() {
		return "Clock";
	}
	Ptr<IEntity> Create() {
		return new ClockEntity();
	}
} ClockEntityClass;
EntityClass* ClockEntity::Class = &ClockEntityClass;

//--
ClockEntity::ClockEntity() {
	

}

//--
void ClockEntity::Update(TimeStep Time) {
	this->_time += Time;
}

//--
void ClockEntity::Clone(Ptr<IEntity> To) {
	IEntity* t = To;
	((ClockEntity*)t)->_time = this->_time;
}

//--
Ptr<IVisual> ClockEntity::CreateVisual(VisualParameters Params) {
	return new ClockEntityVisual(Params);
}

//--
EntityClass* ClockEntity::GetClass() {
	return ClockEntity::Class;
}

//--
TimeStep ClockEntity::GetTime() {
	return this->_time;
}

//--
void ClockEntity::SetTime(TimeStep Time) {
	this->_time = Time;
}

//--
ClockEntityVisual::ClockEntityVisual(VisualParameters Params) {
	this->_params = Params;
	this->_font = this->_params.Device->getGUIEnvironment()->getFont("Font/LucidaConsole10.png");
	this->_target.SetOwner(this);
}

//--
void ClockEntityVisual::Update(Ptr<IEntity> Entity) {
	this->_target = Cast<ClockEntity>(Entity);
}

//--
void ClockEntityVisual::Render() {
	scene::ISceneManager* smgr = this->_params.SceneManager;
	video::IVideoDriver* driver = smgr->getVideoDriver();

	// Get screen position
	core::vector3df pos(0.0f, 0.0f, 0.0f);
	core::position2di spos = smgr->getSceneCollisionManager()->getScreenCoordinatesFrom3DPosition(pos);
	core::rect<s32> fpos(spos.X, spos.Y, spos.X, spos.Y);

	// Format time
	TimeStep time = this->_target->GetTime();
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
	this->_font->draw(timestring.c_str(), fpos, video::SColor(255, 255, 255, 255), true, true);
}