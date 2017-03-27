/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: lexical_cast.h
---------------------------------------------------
*/

#ifndef _LEXICAL_CAST_
#define _LEXICAL_CAST_

#include "any_cast.h"
#include "string_utility.h"
#include "type_traits.h"

namespace lunar {

//Template helper functions

template <typename _Ty>
_Ty cast_helper(const std::string &_String,bool _Throw,tags::type_tag) {
	return any_cast<_Ty>(_String,_Throw); //Default conversion
}

template <typename _Ty>
_Ty cast_helper(const std::string &_String,bool _Throw,tags::bool_tag) {

	//Special values for 1
	if (string_utility::compare(_String,"true") || string_utility::
		compare(_String,"yes") || string_utility::compare(_String,"on"))
			return true;
	
	//Special values for 0
	if (string_utility::compare(_String,"false") || string_utility::
		compare(_String,"no") || string_utility::compare(_String,"off"))
			return false;

return any_cast<bool>(_String,_Throw);
}

template <typename _Ty>
_Ty cast_helper(const std::string &_String,bool _Throw,tags::string_tag) {
	return _String; //No conversions required
}

template <typename _Ty>
_Ty cast_helper(const std::string &_String,bool _Throw,tags::color_tag) {

string_utility::tokens _Tokens;
string_utility::tokenize(_String,",",_Tokens);

	//Color as hex rgba value
	if (_Tokens.size()==1) {
	
		if ((_Tokens[0].size()>1 && _Tokens[0][0]=='#') ||
			(_Tokens[0].size()>2 && _Tokens[0][0]=='0' &&
				(_Tokens[0][1]=='x' || _Tokens[0][1]=='X'))) {
		
		std::stringstream _Stream;
		_Stream<<std::hex<<_Tokens[0]; //Hex mode
		unsigned int _Rgba;
		
			if (_Stream>>_Rgba) return color(_Rgba);
		}
	}
	
	//Color as red, green, blue components
	else if (_Tokens.size()==3) return color(
		(unsigned char)any_cast<unsigned short>(_Tokens[0],_Throw),
			(unsigned char)any_cast<unsigned short>(_Tokens[1],_Throw),
				(unsigned char)any_cast<unsigned short>(_Tokens[2],_Throw));
	
	//Color as red, green, blue, alpha components
	else if (_Tokens.size()==4) return color(
		(unsigned char)any_cast<unsigned short>(_Tokens[0],_Throw),
			(unsigned char)any_cast<unsigned short>(_Tokens[1],_Throw),
				(unsigned char)any_cast<unsigned short>(_Tokens[2],_Throw),
					any_cast<float>(_Tokens[3],_Throw));
	
	//Invalid data
	if (_Throw) throw std::bad_cast();

return color::WHITE;
}

template <typename _Ty>
_Ty cast_helper(const std::string &_String,bool _Throw,tags::vector2_tag) {

string_utility::tokens _Tokens;
string_utility::tokenize(_String,",",_Tokens);

	//Vector2 as scalar
	if (_Tokens.size()==1) return vector2(
		any_cast<float>(_Tokens[0],_Throw));
	
	//Vector2 as x, y components
	if (_Tokens.size()==2) return vector2(
		any_cast<float>(_Tokens[0],_Throw),
			any_cast<float>(_Tokens[1],_Throw));
	
	//Invalid data
	if (_Throw) throw std::bad_cast();

return vector2::ZERO;
}

//Template function

template <typename _Ty>
_Ty lexical_cast(const std::string &_String,bool _Throw=false) {
	return cast_helper<_Ty>(_String,_Throw,typename type_traits<_Ty>::category());
}

} //namespace lunar

#endif