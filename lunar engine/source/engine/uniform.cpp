/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: uniform.cpp
---------------------------------------------------
*/

#include "uniform.h"

namespace lunar {

//Protected

uniform::uniform(const std::string &_Name,unsigned int _Components,
	unsigned int _Size):shadervariable(_Name,_Components),
		_Size(_Size) {
	//Empty
}

uniform::uniform(const std::string &_Name,int *_Data,
	unsigned int _Components,unsigned int _Size):
		shadervariable(_Name,_Data,_Components),_Size(_Size) {
	//Empty
}

uniform::uniform(const std::string &_Name,float *_Data,
	unsigned int _Components,unsigned int _Size):
		shadervariable(_Name,_Data,_Components),_Size(_Size) {
	//Empty
}

uniform::uniform(const uniform &_Uniform):shadervariable(_Uniform) {
	copy(_Uniform);
}

uniform::~uniform() {
	//Empty
}

void uniform::copy(const uniform &_Uniform) {
	_Size=_Uniform._Size;
}

//Public

uniform &uniform::operator=(const uniform &_Uniform) {

	//Worth doing
	if (this!=&_Uniform) {
	
	shadervariable::operator=(_Uniform); //Invoke base operator=
	copy(_Uniform);
	}

return *this;
}

void uniform::size(unsigned int _Size) {
	this->_Size=_Size;
}

unsigned int uniform::size() const {
	return _Size;
}

} //namespace lunar