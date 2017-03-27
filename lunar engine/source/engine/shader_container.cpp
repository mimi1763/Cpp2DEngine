/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: shader_container.cpp
---------------------------------------------------
*/

#include "shader_container.h"

namespace lunar {

//Shader data
//Protected

void shader_data::copy(const shader_data &_Shader_data) {
	_Type=_Shader_data._Type;
}

//Public

shader_data::shader_data(const std::string &_Name,const std::string &_Filename,
	const TYPE &_Type):file_data(_Name,_Filename),_Type(_Type) {
	//Empty
}

shader_data::shader_data(const shader_data &_Shader_data):
	file_data(_Shader_data) {
	copy(_Shader_data);
}

shader_data::~shader_data() {
	//Empty
}

shader_data &shader_data::operator=(const shader_data &_Shader_data) {

	//Worth doing
	if (this!=&_Shader_data) {
	
	file_data::operator=(_Shader_data); //Invoke base operator=
	copy(_Shader_data);
	}

return *this;
}

void shader_data::type(const TYPE &_Type) {
	this->_Type=_Type;
}

shader_data::TYPE shader_data::type() const {
	return _Type;
}

//Shader container
//Private

//This instance is allocated when resource_manager is initialized
//This instance is deallocated when resource_manager is destroyed
shader_container *shader_container::_Instance=0;

//Protected

void shader_container::copy(const shader_container &_Shader_container) {
	//Empty - override default memcpy behaviour
}

//Public

shader_container::shader_container() {
	//Empty
}

shader_container::shader_container(const shader_container &_Shader_container):
	file_container(_Shader_container) {
	copy(_Shader_container);
}

shader_container::~shader_container() {
	//Empty
}

shader_container &shader_container::operator=(const shader_container &_Shader_container) {

	//Worth doing
	if (this!=&_Shader_container) {
	
	file_container::operator=(_Shader_container); //Invoke base operator=
	copy(_Shader_container);
	}

return *this;
}

shader_container *shader_container::instance() {
	return _Instance;
}

shader_data *shader_container::add_data(const shader_data &_Shader_data) {

shader_data *_Data=static_cast<shader_data*>(
	data(_Shader_data.name()));

	//Data already exists
	if (_Data) {
	
	*_Data=_Shader_data; //Replace with new data
	return _Data;
	}

_Data=new shader_data(_Shader_data); //Make copy
_Mydata.push_back(_Data);

_Mydatamap.insert(_Datamap::value_type(_Data->name(),
	(unsigned int)_Mydata.size()-1));
return _Data;
}

} //namespace lunar