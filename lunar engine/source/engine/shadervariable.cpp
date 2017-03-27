/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: shadervariable.cpp
---------------------------------------------------
*/

#include "shadervariable.h"

namespace lunar {

//Protected

shadervariable::shadervariable(const std::string &_Name,
	unsigned int _Components):_Name(_Name),_Location(-1),
		_Data(0),_Components(_Components),_Data_type(SHADER_INT),
			_Auto_update(false),_Status(NEED_UPDATE),
				_Update_data(true) {
	//Empty
}

shadervariable::shadervariable(const std::string &_Name,int *_Data,
	unsigned int _Components):_Name(_Name),_Location(-1),
		_Data((void*)_Data),_Components(_Components),
			_Data_type(SHADER_INT),_Auto_update(false),
				_Status(NEED_UPDATE),_Update_data(true) {
	//Empty
}

shadervariable::shadervariable(const std::string &_Name,float *_Data,
	unsigned int _Components):_Name(_Name),_Location(-1),
		_Data((void*)_Data),_Components(_Components),
			_Data_type(SHADER_FLOAT),_Auto_update(false),
				_Status(NEED_UPDATE),_Update_data(true) {
	//Empty
}

shadervariable::shadervariable(const shadervariable &_Shadervariable) {
	copy(_Shadervariable);
}

shadervariable::~shadervariable() {
	//Empty
}

void shadervariable::copy(const shadervariable &_Shadervariable) {

_Name=_Shadervariable._Name;
_Location=_Shadervariable._Location;

_Data=_Shadervariable._Data;
_Components=_Shadervariable._Components;
_Data_type=_Shadervariable._Data_type;
_Auto_update=_Shadervariable._Auto_update;

_Status=NEED_UPDATE; //Update me
_Update_data=true; //Update my data next frame
}

//Public

shadervariable &shadervariable::operator=(const shadervariable &_Shadervariable) {

	//Worth doing
	if (this!=&_Shadervariable) copy(_Shadervariable);

return *this;
}

bool shadervariable::operator==(const shadervariable &_Shadervariable) const {
	return (_Name==_Shadervariable._Name);
}

bool shadervariable::operator!=(const shadervariable &_Shadervariable) const {
	return !(*this==_Shadervariable);
}

void shadervariable::name(const std::string &_Name) {

	if (this->_Name!=_Name) {
	
	this->_Name=_Name;
	_Status=NEED_UPDATE; //Update me
	_Update_data=true; //Update my data next frame
	}
}

const std::string &shadervariable::name() const {
	return _Name;
}

unsigned int shadervariable::location() const {
	return _Location;
}

void shadervariable::data(int *_Data) {

	if (this->_Data!=(void*)_Data) {
	
	this->_Data=(void*)_Data;
	_Data_type=SHADER_INT; //Tag this as int
	_Update_data=true; //Update my data next frame
	}
}

void shadervariable::data(float *_Data) {

	if (this->_Data!=(void*)_Data) {
	
	this->_Data=(void*)_Data;
	_Data_type=SHADER_FLOAT; //Tag this as float
	_Update_data=true; //Update my data next frame
	}
}

void *shadervariable::data() const {
	return _Data;
}

void shadervariable::components(unsigned int _Components) {
	this->_Components=_Components;
}

unsigned int shadervariable::components() const {
	return _Components;
}

shadervariable::DATA_TYPE shadervariable::data_type() const {
	return _Data_type;
}

void shadervariable::auto_update(bool _Auto_update) {
	this->_Auto_update=_Auto_update;
}

bool shadervariable::auto_update() const {
	return _Auto_update;
}

shadervariable::STATUS shadervariable::status() const {
	return _Status;
}

void shadervariable::update_data() {

	//Update my data next frame
	if (_Status==OKAY) _Update_data=true;
}

} //namespace lunar