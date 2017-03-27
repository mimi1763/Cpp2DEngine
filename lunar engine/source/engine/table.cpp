/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: table.cpp
---------------------------------------------------
*/

#include "table.h"

namespace lunar {

//Protected

void table::copy(const table &_Table) {

_Name=_Table._Name;
_Name_column=_Table._Name_column;
}

//Public

table::table(const std::string &_Name):_Name(_Name),
	_Name_column("name") {
	//Empty
}

table::table(const table &_Table) {
	copy(_Table);
}

table::~table() {
	//Empty
}

table &table::operator=(const table &_Table) {

	//Worth doing
	if (this!=&_Table) copy(_Table);

return *this;
}

void table::name(const std::string &_Name) {
	this->_Name=_Name;
}

const std::string &table::name() const {
	return _Name;
}

void table::columns(const std::string &_Name_column) {
	this->_Name_column=_Name_column;
}

const std::string &table::name_column() const {
	return _Name_column;
}

} //namespace lunar