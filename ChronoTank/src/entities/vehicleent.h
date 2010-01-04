#ifndef CTANK_VEHICLE_H
#define CTANK_VEHICLE_H

#include <irrlicht.h>

#include "nullity/object.h"

namespace ctank {

	/// Enumeration of vehicle control values that can be set.
	enum VehicleControl {
		VehicleControlFoward,
		VehicleControlTurn,
	};

	/// Interface to a vehicle that moves n stuff.
	class IVehicle : public nullity::IObject {
	public:

		/// Sets a control value for the vehicle.
		virtual void			SetVehicleControl(VehicleControl Key, float Value) = 0;

	};

	/// Creates the tank used by the player.
	IVehicle*			CreatePlayerTank();

}

#endif