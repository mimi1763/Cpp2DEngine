/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: any_cast.h
---------------------------------------------------
*/

#ifndef _ANY_CAST_
#define _ANY_CAST_

#include <string>
#include <sstream>

namespace lunar {

//Converts std::string to templated type
template <typename _Ty>
_Ty any_cast(const std::string &_String,bool _Throw=false) {

std::stringstream _Stream;
_Stream<<_String;

_Ty _Value;

	if (!(_Stream>>_Value)) {
	
		if (_Throw) throw std::bad_cast();
	
	return _Ty(); //Default value for templated type
	}

return _Value;
}

//Converts templated type to std::string
template <typename _Ty>
std::string any_cast(const _Ty &_Value,bool _Throw=false) {

std::stringstream _Stream;

	if (!(_Stream<<_Value)) {
	
		if (_Throw) throw std::bad_cast();
	
	return std::string(); //Default value for std::string
	}

return _Stream.str();
}

} //namespace lunar

#endif