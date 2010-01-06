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
class PlayerTank : public IUnit {
public:
	PlayerTank();

	void			Update(TimeStep Time);
	void			Clone(Ptr<Entity> To);
	Ptr<IVisual>	CreateVisual(VisualParameters Params);
	EntityClass*	GetClass();

	void		Turn(float Amount);
	void		Move(float Amount);

};