/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: font.cpp
---------------------------------------------------
*/

#include "font.h"

//Dependencies
#include "font_manager.h"

namespace lunar {

//Protected

font::font(font_manager *_Owner,font_data *_Font_data):
	manageable(_Owner,_Font_data),_Listbase(0),_Glyphs(0),
		_Height(0),_Advance(0) {
	//Empty
}

font::font(const font &_Font):manageable(_Font) {
	copy(_Font);
}

font::font(font_manager *_Owner,const font &_Font):
	manageable(_Owner,_Font) {
	copy(_Font);
}

font::~font() {
	//Empty
}

void font::copy(const font &_Font) {

_Listbase=_Font._Listbase;
_Glyphs=_Font._Glyphs;
_Height=_Font._Height;
_Advance=_Font._Advance;

status(_Font._Status); //Inherit status
}

//Public

font &font::operator=(const font &_Font) {

	//Worth doing
	if (this!=&_Font) {
	
	manageable::operator=(_Font); //Invoke base operator=
	copy(_Font);
	}

return *this;
}

void font::data(font_data *_Font_data) {
	manageable::data(_Font_data); //Data contains the correct derived data type
}

font_data *font::data() const {
	return static_cast<font_data*>(manageable::data());
}

unsigned int font::listbase() const {
	return _Listbase;
}

unsigned int font::glyphs() const {
	return _Glyphs;
}

unsigned int font::height() const {
	return _Height;
}

unsigned int *font::advance() const {
	return _Advance;
}

} //namespace lunar