/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: font_table.cpp
---------------------------------------------------
*/

#include "font_table.h"

//Dependencies
#include "font_container.h"
#include "connector.h"
#include "any_cast.h"

namespace lunar {

//Private

//This instance is allocated when resource_manager is initialized
//This instance is deallocated when resource_manager is destroyed
font_table *font_table::_Instance=0;

//Protected

void font_table::copy(const font_table &_Font_table) {

_Face_column=_Font_table._Face_column;
_Width_column=_Font_table._Width_column;
_Height_column=_Font_table._Height_column;
_Horizontalres_column=_Font_table._Horizontalres_column;
_Verticalres_column=_Font_table._Verticalres_column;
_Encoding_column=_Font_table._Encoding_column;

_Font_container=_Font_table._Font_container;
}

//Public

font_table::font_table(font_container *_Font_container):file_table("font"),
	_Face_column("face"),_Width_column("width"),_Height_column("height"),
		_Horizontalres_column("horizontal_res"),_Verticalres_column("vertical_res"),
			_Encoding_column("encoding"),_Font_container(_Font_container) {
	//Empty
}

font_table::font_table(const font_table &_Font_table):
	file_table(_Font_table) {
	copy(_Font_table);
}

font_table::~font_table() {
	//Empty
}

font_table &font_table::operator=(const font_table &_Font_table) {

	//Worth doing
	if (this!=&_Font_table) {
	
	file_table::operator=(_Font_table); //Invoke base operator=
	copy(_Font_table);
	}

return *this;
}

font_table *font_table::instance() {
	return _Instance;
}
	
bool font_table::load(connector *_Connector) {

//Main query (font)
std::string _Query("SELECT `"+_Name_column+"`,`"+_Filename_column+"`,`"+
	_Face_column+"`,`"+_Width_column+"`,`"+_Height_column+"`,`"+
		_Horizontalres_column+"`,`"+_Verticalres_column+"`,`"+
			_Encoding_column+"` FROM `"+_Name+"`");

connector::query_data _Data;

	if (!_Connector->query(_Query,_Data)) return false;
	if (_Data.empty()) return true;

_Font_container->increase_reserve(_Data.size()); //Vector optimization

	for (connector::query_iter _Iter=_Data.begin(),
		_End=_Data.end();_Iter!=_End;++_Iter) {
	
	connector::row_iter _Row_iter=(*_Iter).begin();
	
	std::string _Name=(*_Row_iter);
	std::string _Filename=*(++_Row_iter);
	unsigned int _Face=any_cast<unsigned int>(*(++_Row_iter));
	unsigned int _Width=any_cast<unsigned int>(*(++_Row_iter));
	unsigned int _Height=any_cast<unsigned int>(*(++_Row_iter));
	unsigned int _Horizontal_res=any_cast<unsigned int>(*(++_Row_iter));
	unsigned int _Vertical_res=any_cast<unsigned int>(*(++_Row_iter));
	font_data::ENCODING _Encoding=(font_data::ENCODING)
		any_cast<int>(*(++_Row_iter));
	
	_Font_container->add_data(font_data(_Name,_Filename,_Face,
		_Width,_Height,_Horizontal_res,_Vertical_res,_Encoding));
	}

return true;
}

void font_table::columns(const std::string &_Face_column,const std::string &_Width_column,
	const std::string &_Height_column,const std::string &_Horizontalres_column,
		const std::string &_Verticalres_column,const std::string &_Encoding_column) {

this->_Face_column=_Face_column;
this->_Width_column=_Width_column;
this->_Height_column=_Height_column;
this->_Horizontalres_column=_Horizontalres_column;
this->_Verticalres_column=_Verticalres_column;
this->_Encoding_column=_Encoding_column;
}

const std::string &font_table::face_column() const {
	return _Face_column;
}

const std::string &font_table::width_column() const {
	return _Width_column;
}

const std::string &font_table::height_column() const {
	return _Height_column;
}

const std::string &font_table::horizontalres_column() const {
	return _Horizontalres_column;
}

const std::string &font_table::verticalres_column() const {
	return _Verticalres_column;
}

const std::string &font_table::encoding_column() const {
	return _Encoding_column;
}

} //namespace lunar