/************************************************/
/*	Time Manipulator Implementation     		*/
/************************************************/
#include "timemanipulator.h"

using namespace nullity;

/****************************************/
/*	Time manipulator					*/
/****************************************/
//--
int ITimeManipulator::GetSpecialFlag() {
	return ITimeManipulator::SpecialFlag;
}

/****************************************/
/*	Distort time manipulator			*/
/****************************************/
//--
IDistortTimeManipulator::IDistortTimeManipulator() {
	this->_position = Vector();
	this->_timedistortion = 0.5f;
	this->_radius = 5.0f;
}

//--
void IDistortTimeManipulator::Destroy() {
	delete this;
}

//--
Vector IDistortTimeManipulator::GetPosition() {
	return this->_position;
}

//--
Vector IDistortTimeManipulator::GetRotation() {
	return Vector();
}

//--
void IDistortTimeManipulator::SetPosition(Vector Position) {
	this->_position = Position;
}

//--
void IDistortTimeManipulator::SetRadius(float Length) {
	this->_radius = Length;
}

//--
void IDistortTimeManipulator::SetTimeDistortion(TimeStep Amount) {
	this->_timedistortion = Amount;
}

//--
TimeStep IDistortTimeManipulator::GetTimeDistortion() {
	return this->_timedistortion;
}

//--
float IDistortTimeManipulator::GetRadius() {
	return this->_radius;
}

/****************************************/
/*	Linear distort time manipulator		*/
/****************************************/
//--
class LinearDistortTimeManipulator : public IDistortTimeManipulator {
public:

	TimeStep	GetTimeRateAtPosition(Vector Position);

};

//--
IDistortTimeManipulator* nullity::CreateLinearDistortTimeManipulator() {
	return new LinearDistortTimeManipulator();
}

//--
TimeStep LinearDistortTimeManipulator::GetTimeRateAtPosition(Vector Position) {
	float dis = Position.getDistanceFrom(this->GetPosition());
	float rad = this->GetRadius();
	TimeStep rate = this->GetTimeDistortion();
	if(dis > rad) {
		return 1.0f;
	} else {
		float rdis = dis / rad;
		return ((1.0f - rdis) * rate) + rdis;
	}
}

/****************************************/
/*	Sharp distort time manipulator		*/
/****************************************/
//--
class SharpDistortTimeManipulator : public IDistortTimeManipulator {
public:

	TimeStep	GetTimeRateAtPosition(Vector Position);

};

//--
IDistortTimeManipulator* nullity::CreateSharpDistortTimeManipulator() {
	return new SharpDistortTimeManipulator();
}

//--
TimeStep SharpDistortTimeManipulator::GetTimeRateAtPosition(Vector Position) {
	float dis = Position.getDistanceFrom(this->GetPosition());
	float rad = this->GetRadius();
	TimeStep rate = this->GetTimeDistortion();
	if(dis > rad) {
		return 1.0f;
	} else {
		return rate;
	}
}