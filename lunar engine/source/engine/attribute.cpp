/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: attribute.cpp
---------------------------------------------------
*/

#include "attribute.h"

namespace lunar {

//Protected

attribute::attribute(const std::string &_Name,unsigned int _Components,
	unsigned int _Stride,bool _Normalized):
		shadervariable(_Name,_Components),_Stride(_Stride),
			_Normalized(_Normalized) {
	_Auto_update=true; //Default for attribute
}

attribute::attribute(const std::string &_Name,int *_Data,
	unsigned int _Components,unsigned int _Stride,
		bool _Normalized):shadervariable(_Name,_Data,_Components),
			_Stride(_Stride),_Normalized(_Normalized) {
	_Auto_update=true; //Default for attribute
}

attribute::attribute(const std::string &_Name,float *_Data,
	unsigned int _Components,unsigned int _Stride,
		bool _Normalized):shadervariable(_Name,_Data,_Components),
			_Stride(_Stride),_Normalized(_Normalized) {
	_Auto_update=true; //Default for attribute
}

attribute::attribute(const attribute &_Attribute):
	shadervariable(_Attribute) {

_Auto_update=true; //Default for attribute
copy(_Attribute);
}

attribute::~attribute() {
	//Empty
}

void attribute::copy(const attribute &_Attribute) {

_Stride=_Attribute._Stride;
_Normalized=_Attribute._Normalized;
}

//Public

attribute &attribute::operator=(const attribute &_Attribute) {

	//Worth doing
	if (this!=&_Attribute) {
	
	shadervariable::operator=(_Attribute); //Invoke base operator=
	copy(_Attribute);
	}

return *this;
}

void attribute::stride(unsigned int _Stride) {
	this->_Stride=_Stride;
}

unsigned int attribute::stride() const {
	return _Stride;
}

void attribute::normalized(bool _Normalized) {
	this->_Normalized=_Normalized;
}

bool attribute::normalized() const {
	return _Normalized;
}

} //namespace lunar