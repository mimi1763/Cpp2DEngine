/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: resource_container.cpp
---------------------------------------------------
*/

#include "resource_container.h"

namespace lunar {

//Resource data
//Protected

resource_data::resource_data(const std::string &_Name):_Name(_Name) {
	//Empty
}

resource_data::resource_data(const resource_data &_Resource_data) {
	copy(_Resource_data);
}

resource_data::~resource_data() {
	//Empty
}

resource_data &resource_data::operator=(const resource_data &_Resource_data) {

	//Worth doing
	if (this!=&_Resource_data) copy(_Resource_data);

return *this;
}

void resource_data::copy(const resource_data &_Resource_data) {
	_Name=_Resource_data._Name;
}

//Public

void resource_data::name(const std::string &_Name) {
	this->_Name=_Name;
}

const std::string &resource_data::name() const {
	return _Name;
}

//Resource container
//Protected

resource_container::resource_container() {
	//Empty
}

resource_container::resource_container(const resource_container &_Resource_container) {
	copy(_Resource_container);
}

resource_container::~resource_container() {
	clear();
}

void resource_container::copy(const resource_container &_Resource_container) {
	//Empty - override default memcpy behaviour
}

//Public

resource_container &resource_container::operator=(const resource_container &_Resource_container) {

	//Worth doing
	if (this!=&_Resource_container) copy(_Resource_container);

return *this;
}

void resource_container::increase_reserve(const size_t &_Count) {
	_Mydata.reserve(_Mydata.size()+_Count);
}

resource_data *resource_container::data(unsigned int _Index) const {
	return (_Index<_Mydata.size()?_Mydata[_Index]:0);
}

resource_data *resource_container::data(const std::string &_Name) const {

_Datamap::const_iterator _Iter=_Mydatamap.find(_Name);
return (_Iter!=_Mydatamap.end()?data((*_Iter).second):0);
}

unsigned int resource_container::data_index(const std::string &_Name) const {

_Datamap::const_iterator _Iter=_Mydatamap.find(_Name);
return (_Iter!=_Mydatamap.end()?(*_Iter).second:-1);
}

void resource_container::clear() {

	for (_Data::const_iterator _Iter=_Mydata.begin(),
		_End=_Mydata.end();_Iter!=_End;++_Iter) delete (*_Iter);

_Mydata.clear();
_Mydatamap.clear();
}

resource_container::data_iter resource_container::data_begin() const {
	return _Mydata.begin();
}

resource_container::data_iter resource_container::data_end() const {
	return _Mydata.end();
}

} //namespace lunar