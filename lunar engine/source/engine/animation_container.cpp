/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: animation_container.cpp
---------------------------------------------------
*/

#include "animation_container.h"

namespace lunar {

//Animation data
//Protected

void animation_data::copy(const animation_data &_Animation_data) {
	_Stream=_Animation_data._Stream;
}

//Public

animation_data::animation_data(const std::string &_Name,const std::string &_Filename,
	bool _Stream):file_data(_Name,_Filename),_Stream(_Stream) {
	//Empty
}

animation_data::animation_data(const animation_data &_Animation_data):
	file_data(_Animation_data) {
	copy(_Animation_data);
}

animation_data::~animation_data() {
	//Empty
}

animation_data &animation_data::operator=(const animation_data &_Animation_data) {

	//Worth doing
	if (this!=&_Animation_data) {
	
	file_data::operator=(_Animation_data); //Invoke base operator=
	copy(_Animation_data);
	}

return *this;
}

void animation_data::stream(bool _Stream) {
	this->_Stream=_Stream;
}

bool animation_data::stream() const {
	return _Stream;
}

//Animation container
//Private

//This instance is allocated when resource_manager is initialized
//This instance is deallocated when resource_manager is destroyed
animation_container *animation_container::_Instance=0;

//Protected

void animation_container::copy(const animation_container &_Animation_container) {
	//Empty - override default memcpy behaviour
}

//Public

animation_container::animation_container() {
	//Empty
}

animation_container::animation_container(const animation_container &_Animation_container):
	file_container(_Animation_container) {
	copy(_Animation_container);
}

animation_container::~animation_container() {
	//Empty
}

animation_container &animation_container::operator=(
	const animation_container &_Animation_container) {

	//Worth doing
	if (this!=&_Animation_container) {
	
	file_container::operator=(_Animation_container); //Invoke base operator=
	copy(_Animation_container);
	}

return *this;
}

animation_container *animation_container::instance() {
	return _Instance;
}

animation_data *animation_container::add_data(
	const animation_data &_Animation_data) {

animation_data *_Data=static_cast<animation_data*>(
	data(_Animation_data.name()));

	//Data already exists
	if (_Data) {
	
	*_Data=_Animation_data; //Replace with new data
	return _Data;
	}

_Data=new animation_data(_Animation_data); //Make copy
_Mydata.push_back(_Data);

_Mydatamap.insert(_Datamap::value_type(_Data->name(),
	(unsigned int)_Mydata.size()-1));
return _Data;
}

} //namespace lunar