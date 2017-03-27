/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: sound_container.cpp
---------------------------------------------------
*/

#include "sound_container.h"

namespace lunar {

//Sound data
//Protected

void sound_data::copy(const sound_data &_Sound_data) {

_Stream=_Sound_data._Stream;
_Software=_Sound_data._Software;
_Positional=_Sound_data._Positional;
_Loop=_Sound_data._Loop;
}

//Public

sound_data::sound_data(const std::string &_Name,const std::string &_Filename,
	bool _Stream,bool _Software,bool _Positional,
		bool _Loop):file_data(_Name,_Filename),_Stream(_Stream),
			_Software(_Software),_Positional(_Positional),_Loop(_Loop) {
	//Empty
}

sound_data::sound_data(const sound_data &_Sound_data):
	file_data(_Sound_data) {
	copy(_Sound_data);
}

sound_data::~sound_data() {
	//Empty
}

sound_data &sound_data::operator=(const sound_data &_Sound_data) {

	//Worth doing
	if (this!=&_Sound_data) {
	
	file_data::operator=(_Sound_data); //Invoke base operator=
	copy(_Sound_data);
	}

return *this;
}

void sound_data::stream(bool _Stream) {
	this->_Stream=_Stream;
}

bool sound_data::stream() const {
	return _Stream;
}

void sound_data::software(bool _Software) {
	this->_Software=_Software;
}

bool sound_data::software() const {
	return _Software;
}

void sound_data::positional(bool _Positional) {
	this->_Positional=_Positional;
}

bool sound_data::positional() const {
	return _Positional;
}

void sound_data::loop(bool _Loop) {
	this->_Loop=_Loop;
}

bool sound_data::loop() const {
	return _Loop;
}

//Sound container
//Private

//This instance is allocated when resource_manager is initialized
//This instance is deallocated when resource_manager is destroyed
sound_container *sound_container::_Instance=0;

//Protected

void sound_container::copy(const sound_container &_Sound_container) {
	//Empty - override default memcpy behaviour
}

//Public

sound_container::sound_container() {
	//Empty
}

sound_container::sound_container(const sound_container &_Sound_container):
	file_container(_Sound_container) {
	copy(_Sound_container);
}

sound_container::~sound_container() {
	//Empty
}

sound_container &sound_container::operator=(const sound_container &_Sound_container) {

	//Worth doing
	if (this!=&_Sound_container) {
	
	file_container::operator=(_Sound_container); //Invoke base operator=
	copy(_Sound_container);
	}

return *this;
}

sound_container *sound_container::instance() {
	return _Instance;
}

sound_data *sound_container::add_data(const sound_data &_Sound_data) {

sound_data *_Data=static_cast<sound_data*>(
	data(_Sound_data.name()));

	//Data already exists
	if (_Data) {
	
	*_Data=_Sound_data; //Replace with new data
	return _Data;
	}

_Data=new sound_data(_Sound_data); //Make copy
_Mydata.push_back(_Data);

_Mydatamap.insert(_Datamap::value_type(_Data->name(),
	(unsigned int)_Mydata.size()-1));
return _Data;
}

} //namespace lunar