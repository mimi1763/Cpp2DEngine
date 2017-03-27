/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: affector.cpp
---------------------------------------------------
*/

#include "affector.h"

namespace lunar {

//Protected

affector::affector():_Enabled(true) {
	//Empty
}

affector::affector(const affector &_Affector) {
	copy(_Affector);
}

affector::~affector() {
	//Empty
}

void affector::copy(const affector &_Affector) {
	_Enabled=_Affector._Enabled;
}

//Public

affector &affector::operator=(const affector &_Affector) {

	//Worth doing
	if (this!=&_Affector) copy(_Affector);

return *this;
}

void affector::enable() {
	_Enabled=true;
}

void affector::disable() {
	_Enabled=false;
}

bool affector::enabled() const {
	return _Enabled;
}

} //namespace lunar