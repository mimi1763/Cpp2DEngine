/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: script_property.cpp
---------------------------------------------------
*/

#include "script_property.h"

namespace lunar {

//Public

//Data
//Private

void script_property::data::copy(const data &_Data) {

_Property_name=_Data._Property_name;
_Myvalues=_Data._Myvalues;
}

//Data
//Public

script_property::data::data() {
	//Empty
}

script_property::data::data(const data &_Data) {
	copy(_Data);
}

script_property::data::~data() {
	//Empty
}

script_property::data &script_property::data::operator=(
	const data &_Data) {

	//Worth doing
	if (this!=&_Data) copy(_Data);

return *this;
}

bool script_property::data::operator==(const std::string &_Property_name) const {
	return string_utility::compare(this->_Property_name,_Property_name);
}

bool script_property::data::operator!=(const std::string &_Property_name) const {
	return !(*this==_Property_name);
}

const std::string &script_property::data::property_name() const {
	return _Property_name;
}

unsigned int script_property::data::values() const {
	return (unsigned int)_Myvalues.size();
}

std::string script_property::data::value(unsigned int _Index) const {
	return (_Index<_Myvalues.size()?_Myvalues[_Index]:"");
}

//Property
//Private

script_property::script_property(const std::string &_Name):
	_Name(_Name),_Required_parameters(0),_Variable_parameters(false) {
	//Empty
}
	
script_property::script_property(const script_property &_Script_property) {
	copy(_Script_property);
}

script_property::script_property(const std::string &_Name,
	const script_property &_Script_property):_Name(_Name) {
	copy(_Script_property);
}

script_property::~script_property() {
	//Empty
}

void script_property::copy(const script_property &_Script_property) {

_Name=_Script_property._Name;
_Myparameters=_Script_property._Myparameters;

_Required_parameters=_Script_property._Required_parameters;
_Variable_parameters=_Script_property._Variable_parameters;
}

//Public

script_property &script_property::operator=(const script_property &_Script_property) {

	//Worth doing
	if (this!=&_Script_property) copy(_Script_property);

return *this;
}

const std::string &script_property::name() const {
	return _Name;
}

unsigned int script_property::parameters() const {
	return (unsigned int)_Myparameters.size();
}

unsigned int script_property::add_parameter(const DATA_TYPE &_Data_type,
	const std::string &_Custom_values) {

_Parameter _Myparameter;
_Myparameter._Data_type=_Data_type;
_Myparameter._Custom_values=_Custom_values;
_Myparameters.push_back(_Myparameter);

_Required_parameters=(unsigned int)_Myparameters.size();
return (unsigned int)_Myparameters.size()-1;
}

unsigned int script_property::add_parameter(const std::string &_Custom_values) {
	return add_parameter(DT_CUSTOM,_Custom_values);
}

void script_property::remove_parameter(unsigned int _Index) {

	if (_Index<_Myparameters.size()) {
	
	_Myparameters.erase(_Myparameters.begin()+_Index);
	
		if (_Required_parameters>_Myparameters.size())
			--_Required_parameters;
	}
}

void script_property::clear_parameters() {

_Myparameters.clear();
_Required_parameters=0;
}

void script_property::required_parameters(unsigned int _Required_parameters) {

	//Invalid value
	if (_Required_parameters==0) return;
	
	if (_Required_parameters>_Myparameters.size())
		this->_Required_parameters=(unsigned int)_Myparameters.size();
	else this->_Required_parameters=_Required_parameters;
}

unsigned int script_property::required_parameters() const {
	return _Required_parameters;
}

void script_property::variable_parameters(bool _Variable_parameters) {
	this->_Variable_parameters=_Variable_parameters;
}

bool script_property::variable_parameters() const {
	return _Variable_parameters;
}

} //namespace lunar