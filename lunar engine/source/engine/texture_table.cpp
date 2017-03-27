/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: texture_table.cpp
---------------------------------------------------
*/

#include "texture_table.h"

//Dependencies
#include "texture_container.h"
#include "connector.h"
#include "any_cast.h"

namespace lunar {

//Private

//This instance is allocated when resource_manager is initialized
//This instance is deallocated when resource_manager is destroyed
texture_table *texture_table::_Instance=0;

//Protected

void texture_table::copy(const texture_table &_Texture_table) {

_Tileable_column=_Texture_table._Tileable_column;

_Texture_container=_Texture_table._Texture_container;
}

//Public

texture_table::texture_table(texture_container *_Texture_container):
	file_table("texture"),_Tileable_column("tileable"),
		_Texture_container(_Texture_container) {
	//Empty
}

texture_table::texture_table(const texture_table &_Texture_table):
	file_table(_Texture_table) {
	copy(_Texture_table);
}

texture_table::~texture_table() {
	//Empty
}

texture_table &texture_table::operator=(const texture_table &_Texture_table) {

	//Worth doing
	if (this!=&_Texture_table) {
	
	file_table::operator=(_Texture_table); //Invoke base operator=
	copy(_Texture_table);
	}

return *this;
}

texture_table *texture_table::instance() {
	return _Instance;
}

bool texture_table::load(connector *_Connector) {

//Main query (texture)
std::string _Query("SELECT `"+_Name_column+"`,`"+_Filename_column+"`,`"+
	_Tileable_column+"` FROM `"+_Name+"`");

connector::query_data _Data;

	if (!_Connector->query(_Query,_Data)) return false;
	if (_Data.empty()) return true;

_Texture_container->increase_reserve(_Data.size()); //Vector optimization

	for (connector::query_iter _Iter=_Data.begin(),
		_End=_Data.end();_Iter!=_End;++_Iter) {
	
	connector::row_iter _Row_iter=(*_Iter).begin();
	
	std::string _Name=(*_Row_iter);
	std::string _Filename=*(++_Row_iter);
	bool _Tileable=any_cast<bool>(*(++_Row_iter));
	
	_Texture_container->add_data(texture_data(_Name,_Filename,_Tileable));
	}

return true;
}

void texture_table::columns(const std::string &_Tileable_column) {
	this->_Tileable_column=_Tileable_column;
}

const std::string &texture_table::tileable_column() const {
	return _Tileable_column;
}

} //namespace lunar