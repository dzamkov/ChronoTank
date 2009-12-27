/************************************************/
/*	Time Implementation     					*/
/************************************************/
#include "time.h"

using namespace nullity;

/****************************************/
/*	Time								*/
/****************************************/
//--
Time nullity::After(Time Time, TimeStep Amount) {
	return Time + Amount;
}