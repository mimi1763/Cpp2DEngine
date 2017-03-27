/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: shader_table.cpp
---------------------------------------------------
*/

#include "shader_table.h"

//Dependencies
#include "shader_container.h"
#include "connector.h"
#include "any_cast.h"

namespace lunar {

//Private

//This instance is allocated when resource_manager is initialized
//This instance is deallocated when resource_manager is destroyed
shader_table *shader_table::_Instance=0;

//Protected

void shader_table::copy(const shader_table &_Shader_table) {

_Type_column=_Shader_table._Type_column;
_Shader_container=_Shader_table._Shader_container;
}

//Public

shader_table::shader_table(shader_container *_Shader_container):
	file_table("shader"),_Type_column("type"),_Shader_container(
		_Shader_container) {
	//Empty
}

shader_table::shader_table(const shader_table &_Shader_table):
	file_table(_Shader_table) {
	copy(_Shader_table);
}

shader_table::~shader_table() {
	//Empty
}

shader_table &shader_table::operator=(const shader_table &_Shader_table) {

	//Worth doing
	if (this!=&_Shader_table) {
	
	file_table::operator=(_Shader_table); //Invoke base operator=
	copy(_Shader_table);
	}

return *this;
}

shader_table *shader_table::instance() {
	return _Instance;
}

bool shader_table::load(connector *_Connector) {

//Main query (shader)
std::string _Query("SELECT `"+_Name_column+"`,`"+_Filename_column+"`,`"+
	_Type_column+"` FROM `"+_Name+"`");

connector::query_data _Data;

	if (!_Connector->query(_Query,_Data)) return false;
	if (_Data.empty()) return true;

_Shader_container->increase_reserve(_Data.size()); //Vector optimization

	for (connector::query_iter _Iter=_Data.begin(),
		_End=_Data.end();_Iter!=_End;++_Iter) {
	
	connector::row_iter _Row_iter=(*_Iter).begin();
	
	std::string _Name=(*_Row_iter);
	std::string _Filename=*(++_Row_iter);
	shader_data::TYPE _Type=(shader_data::TYPE)
		any_cast<int>(*(++_Row_iter));
	
	_Shader_container->add_data(shader_data(_Name,_Filename,_Type));
	}

return true;
}

void shader_table::columns(const std::string &_Type_column) {
	this->_Type_column=_Type_column;
}

const std::string &shader_table::type_column() const {
	return _Type_column;
}

} //namespace lunar