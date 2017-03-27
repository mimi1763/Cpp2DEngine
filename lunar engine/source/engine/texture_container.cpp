/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: texture_container.cpp
---------------------------------------------------
*/

#include "texture_container.h"

namespace lunar {

//Texture data
//Protected

void texture_data::copy(const texture_data &_Texture_data) {
	_Tileable=_Texture_data._Tileable;
}

//Public

texture_data::texture_data(const std::string &_Name,const std::string &_Filename,
	bool _Tileable):file_data(_Name,_Filename),_Tileable(_Tileable) {
	//Empty
}

texture_data::texture_data(const texture_data &_Texture_data):
	file_data(_Texture_data) {
	copy(_Texture_data);
}

texture_data::~texture_data() {
	//Empty
}

texture_data &texture_data::operator=(const texture_data &_Texture_data) {

	//Worth doing
	if (this!=&_Texture_data) {
	
	file_data::operator=(_Texture_data); //Invoke base operator=
	copy(_Texture_data);
	}

return *this;
}

void texture_data::tileable(bool _Tileable) {
	this->_Tileable=_Tileable;
}

bool texture_data::tileable() const {
	return _Tileable;
}

//Texture container
//Private

//This instance is allocated when resource_manager is initialized
//This instance is deallocated when resource_manager is destroyed
texture_container *texture_container::_Instance=0;

//Protected

void texture_container::copy(const texture_container &_Texture_container) {
	//Empty - override default memcpy behaviour
}

//Public

texture_container::texture_container() {
	//Empty
}

texture_container::texture_container(const texture_container &_Texture_container):
	file_container(_Texture_container) {
	copy(_Texture_container);
}

texture_container::~texture_container() {
	//Empty
}

texture_container &texture_container::operator=(const texture_container &_Texture_container) {

	//Worth doing
	if (this!=&_Texture_container) {
	
	file_container::operator=(_Texture_container); //Invoke base operator=
	copy(_Texture_container);
	}

return *this;
}

texture_container *texture_container::instance() {
	return _Instance;
}

texture_data *texture_container::add_data(const texture_data &_Texture_data) {

texture_data *_Data=static_cast<texture_data*>(
	data(_Texture_data.name()));

	//Data already exists
	if (_Data) {
	
	*_Data=_Texture_data; //Replace with new data
	return _Data;
	}

_Data=new texture_data(_Texture_data); //Make copy
_Mydata.push_back(_Data);

_Mydatamap.insert(_Datamap::value_type(_Data->name(),
	(unsigned int)_Mydata.size()-1));
return _Data;
}

} //namespace lunar