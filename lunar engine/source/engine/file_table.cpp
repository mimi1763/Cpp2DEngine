/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: file_table.cpp
---------------------------------------------------
*/

#include "file_table.h"

//Dependencies
#include "file_container.h"
#include "connector.h"

namespace lunar {

//Private

//This instance is allocated when resource_manager is initialized
//This instance is deallocated when resource_manager is destroyed
file_table *file_table::_Instance=0;

//Protected

void file_table::copy(const file_table &_File_table) {

_Filename_column=_File_table._Filename_column;
_File_container=_File_table._File_container;
}

//Public

file_table::file_table(const std::string &_Name,file_container *_File_container):
	table(_Name),_Filename_column("filename"),_File_container(_File_container) {
	//Empty
}

file_table::file_table(const file_table &_File_table):
	table(_File_table) {
	copy(_File_table);
}

file_table::~file_table() {
	//Empty
}

file_table &file_table::operator=(const file_table &_File_table) {

	//Worth doing
	if (this!=&_File_table) {
	
	table::operator=(_File_table); //Invoke base operator=
	copy(_File_table);
	}

return *this;
}

file_table *file_table::instance() {
	return _Instance;
}

bool file_table::load(connector *_Connector) {

	//File container can be null for derived tables
	if (!_File_container) return false;

//Main query (file)
std::string _Query("SELECT `"+_Name_column+"`,`"+_Filename_column+
	"` FROM `"+_Name+"`");

connector::query_data _Data;

	if (!_Connector->query(_Query,_Data)) return false;
	if (_Data.empty()) return true;

_File_container->increase_reserve(_Data.size()); //Vector optimization

	for (connector::query_iter _Iter=_Data.begin(),
		_End=_Data.end();_Iter!=_End;++_Iter) {
	
	connector::row_iter _Row_iter=(*_Iter).begin();
	
	std::string _Name=(*_Row_iter);
	std::string _Filename=*(++_Row_iter);
	
	_File_container->add_data(file_data(_Name,_Filename));
	}

return true;
}

void file_table::columns(const std::string &_Filename_column) {
	this->_Filename_column=_Filename_column;
}

const std::string &file_table::filename_column() const {
	return _Filename_column;
}

} //namespace lunar