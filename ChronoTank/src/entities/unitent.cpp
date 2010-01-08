/************************************************/
/*	Unit Implementation			     			*/
/************************************************/
#include <irrlicht.h>
#include <sstream>
#include <iomanip>

#include "../nullity/visual.h"

#include "unitent.h"

using namespace nullity;
using namespace ctank;
using namespace irr;

/****************************************/
/*	Player Tank 						*/
/****************************************/
//--
SIMPLE_INTERFACE_CLASS(IUnit);

//--
class PlayerTank : 
	public IUnit,
	public IEntityInterface,
	public IDynamicEntity,
	public IVisualEntity, 
	public IInterface {
public:
	DECLARE_INTERFACE_CLASS(PlayerTank)

	void			Update(TimeStep Time);
	Ptr<Entity>		Clone();
	Ptr<IVisual>	CreateVisual(VisualParameters Params);

	void		Turn(float Amount);
	void		Move(float Amount);
};

//--
BEGIN_INTERFACE_CLASS
	INTERFACE_CLASS_NAME(PlayerTank)
	BEGIN_INTERFACE_CLASS_BASES(PlayerTank)
		INTERFACE_CLASS_BASE(IUnit)
		INTERFACE_CLASS_BASE(IVisualEntity)
		INTERFACE_CLASS_BASE(IDynamicEntity)
		INTERFACE_CLASS_BASE(IEntityInterface)
	END_INTERFACE_CLASS_BASES
END_INTERFACE_CLASS(PlayerTank)

//--
Ptr<Entity> ctank::CreatePlayerTank() {
	Ptr<Entity> e = new Entity();
	PlayerTank* pt = new PlayerTank(e);
	e->Init(pt);
	return e;
}

//--
PlayerTank::PlayerTank(IObject* Owner) : 
IUnit(Owner), 
IVisualEntity(Owner), 
IDynamicEntity(Owner), 
IEntityInterface(Owner), 
IInterface(Owner) 
{
}

//--
Ptr<Entity> PlayerTank::Clone() {
	Ptr<Entity> e = new Entity();
	PlayerTank* pt = new PlayerTank(e);
	e->Init(pt);
	return e;
}

//--
Ptr<IVisual> PlayerTank::CreateVisual(VisualParameters Params) {
	io::IReadFile* vissrc = Params.Device->getFileSystem()->createAndOpenFile("ChronoTank/Basic/Placeholder.3ds");
	Ptr<IVisual> vis = LoadVisualFromFile(Params, vissrc);
	vissrc->drop();
	return vis;
}

//--
void PlayerTank::Turn(float Amount) {

}

//--
void PlayerTank::Move(float Amount) {

}