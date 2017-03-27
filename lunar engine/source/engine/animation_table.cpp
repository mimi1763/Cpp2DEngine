/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: animation_table.cpp
---------------------------------------------------
*/

#include "animation_table.h"

//Dependencies
#include "animation_container.h"
#include "connector.h"
#include "any_cast.h"

namespace lunar {

//Private

//This instance is allocated when resource_manager is initialized
//This instance is deallocated when resource_manager is destroyed
animation_table *animation_table::_Instance=0;

//Protected

void animation_table::copy(const animation_table &_Animation_table) {

_Stream_column=_Animation_table._Stream_column;

_Animation_container=_Animation_table._Animation_container;
}

//Public

animation_table::animation_table(animation_container *_Animation_container):
	file_table("animation"),_Stream_column("stream"),
		_Animation_container(_Animation_container) {
	//Empty
}

animation_table::animation_table(const animation_table &_Animation_table):
	file_table(_Animation_table) {
	copy(_Animation_table);
}

animation_table::~animation_table() {
	//Empty
}

animation_table &animation_table::operator=(const animation_table &_Animation_table) {

	//Worth doing
	if (this!=&_Animation_table) {
	
	file_table::operator=(_Animation_table); //Invoke base operator=
	copy(_Animation_table);
	}

return *this;
}

animation_table *animation_table::instance() {
	return _Instance;
}

bool animation_table::load(connector *_Connector) {

//Main query (animation)
std::string _Query("SELECT `"+_Name_column+"`,`"+_Filename_column+"`,`"+
	_Stream_column+"` FROM `"+_Name+"`");

connector::query_data _Data;

	if (!_Connector->query(_Query,_Data)) return false;
	if (_Data.empty()) return true;

_Animation_container->increase_reserve(_Data.size()); //Vector optimization

	for (connector::query_iter _Iter=_Data.begin(),
		_End=_Data.end();_Iter!=_End;++_Iter) {
	
	connector::row_iter _Row_iter=(*_Iter).begin();
	
	std::string _Name=(*_Row_iter);
	std::string _Filename=*(++_Row_iter);
	bool _Stream=any_cast<bool>(*(++_Row_iter));
	
	_Animation_container->add_data(animation_data(_Name,_Filename,_Stream));
	}

return true;
}

void animation_table::columns(const std::string &_Stream_column) {
	this->_Stream_column=_Stream_column;
}

const std::string &animation_table::stream_column() const {
	return _Stream_column;
}

} //namespace lunar