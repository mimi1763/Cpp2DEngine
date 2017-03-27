/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: font_container.cpp
---------------------------------------------------
*/

#include "font_container.h"

namespace lunar {

//Font data
//Protected

void font_data::copy(const font_data &_Font_data) {

_Face=_Font_data._Face;
_Width=_Font_data._Width;
_Height=_Font_data._Height;
_Horizontal_res=_Font_data._Horizontal_res;
_Vertical_res=_Font_data._Vertical_res;
_Encoding=_Font_data._Encoding;
}

//Public

font_data::font_data(const std::string &_Name,const std::string &_Filename,
	unsigned int _Face,unsigned int _Width,unsigned int _Height,
		unsigned int _Horizontal_res,unsigned int _Vertical_res,
			const ENCODING &_Encoding):file_data(_Name,_Filename),
				_Face(_Face),_Width(_Width),_Height(_Height),_Vertical_res(_Vertical_res),
					_Horizontal_res(_Horizontal_res),_Encoding(_Encoding) {
	//Empty
}

font_data::font_data(const font_data &_Font_data):
	file_data(_Font_data) {
	copy(_Font_data);
}

font_data::~font_data() {
	//Empty
}

font_data &font_data::operator=(const font_data &_Font_data) {

	//Worth doing
	if (this!=&_Font_data) {
	
	file_data::operator=(_Font_data); //Invoke base operator=
	copy(_Font_data);
	}

return *this;
}

void font_data::face(unsigned int _Face) {
	this->_Face=_Face;
}

unsigned int font_data::face() const {
	return _Face;
}

void font_data::size(unsigned int _Width,unsigned int _Height) {

this->_Width=_Width;
this->_Height=_Height;
}

unsigned int font_data::width() const {
	return _Width;
}

unsigned int font_data::height() const {
	return _Height;
}

void font_data::resolution(unsigned int _Horizontal_res,
	unsigned int _Vertical_res) {

this->_Horizontal_res=_Horizontal_res;
this->_Vertical_res=_Vertical_res;
}

unsigned int font_data::horizontal_res() const {
	return _Horizontal_res;
}

unsigned int font_data::vertical_res() const {
	return _Vertical_res;
}

void font_data::encoding(const ENCODING &_Encoding) {
	this->_Encoding=_Encoding;
}

font_data::ENCODING font_data::encoding() const {
	return _Encoding;
}

//Font container
//Private

//This instance is allocated when resource_manager is initialized
//This instance is deallocated when resource_manager is destroyed
font_container *font_container::_Instance=0;

//Protected

void font_container::copy(const font_container &_Font_container) {
	//Empty - override default memcpy behaviour
}

//Public

font_container::font_container() {
	//Empty
}

font_container::font_container(const font_container &_Font_container):
	file_container(_Font_container) {
	copy(_Font_container);
}

font_container::~font_container() {
	//Empty
}

font_container &font_container::operator=(const font_container &_Font_container) {

	//Worth doing
	if (this!=&_Font_container) {
	
	file_container::operator=(_Font_container); //Invoke base operator=
	copy(_Font_container);
	}

return *this;
}

font_container *font_container::instance() {
	return _Instance;
}

font_data *font_container::add_data(const font_data &_Font_data) {

font_data *_Data=static_cast<font_data*>(
	data(_Font_data.name()));

	//Data already exists
	if (_Data) {
	
	*_Data=_Font_data; //Replace with new data
	return _Data;
	}

_Data=new font_data(_Font_data); //Make copy
_Mydata.push_back(_Data);

_Mydatamap.insert(_Datamap::value_type(_Data->name(),
	(unsigned int)_Mydata.size()-1));
return _Data;
}

} //namespace lunar