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
SIMPLE_INTERFACE_CLASS(IClockEntity)

//--
class ClockEntityVisual : public IVisual {
public:
	ClockEntityVisual(VisualParameters Params);

	void	Update(Interface* MainInterface);
	void	Render();

private:
	ClockEntity*		_target;
	VisualParameters	_params;
	gui::IGUIFont*		_font;
};

//--
BEGIN_INTERFACE_CLASS(ClockEntity)
	INTERFACE_CLASS_NAME(ClockEntity)
	BEGIN_INTERFACE_CLASS_BASES(ClockEntity)
		INTERFACE_CLASS_BASE(IClockEntity)
		INTERFACE_CLASS_BASE(IVisualEntity)
		INTERFACE_CLASS_BASE(IDynamicEntity)
		INTERFACE_CLASS_BASE(IEntityInterface)
	END_INTERFACE_CLASS_BASES
END_INTERFACE_CLASS(ClockEntity)

//--
ClockEntity::ClockEntity(Ptr<IObject> Owner) :
IClockEntity(Owner), 
IVisualEntity(Owner), 
IDynamicEntity(Owner), 
IEntityInterface(Owner),
Interface(ClockEntity::Class, Owner)
{
	this->_time = 0.0f;
}

//--
void ClockEntity::Update(TimeStep Time) {
	this->_time += Time;
}

//--
Ptr<Entity> ClockEntity::Clone() {
	Ptr<Entity> e = new Entity();
	ClockEntity* ce = new ClockEntity(Ptr<IObject>(e));
	ce->_time = this->_time;
	e->Init(ce);
	return e;
}

//--
Ptr<IVisual> ClockEntity::CreateVisual(VisualParameters Params) {
	return new ClockEntityVisual(Params);
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
}

//--
void ClockEntityVisual::Update(Interface* MainInterface) {
	this->_target = (ClockEntity*)MainInterface;
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