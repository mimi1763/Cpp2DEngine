/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: script_class.cpp
---------------------------------------------------
*/

#include "script_class.h"

//Dependencies
#include "script_scope.h"

namespace lunar {

//Public

//Object
//Private

void script_class::object::copy(const object &_Object) {

copy_linear(_Object);
_Myobjects=_Object._Myobjects;
}

void script_class::object::copy_linear(const object &_Object) {

_Class_name=_Object._Class_name;
_Name=_Object._Name;

_Tag=_Object._Tag;
_Parent=_Object._Parent;
_Depth=_Object._Depth;
_Templated=_Object._Templated;

_Mydata=_Object._Mydata;
}

void script_class::object::append(const object &_Object) {

	//Worth doing
	if (this!=&_Object) {
	
	//Add objects
	_Myobjects.insert(_Myobjects.end(),
		_Object._Myobjects.begin(),_Object._Myobjects.end());
	
	//Add data
	_Mydata.insert(_Mydata.end(),
		_Object._Mydata.begin(),_Object._Mydata.end());
	}
}

void script_class::object::append(const _Objects &_Myobjects) {

	for (_Objects::const_iterator _Iter=_Myobjects.begin(),
		_End=_Myobjects.end();_Iter!=_End;++_Iter)
			append(*_Iter);
}

void script_class::object::append_front(const _Objects &_Myobjects) {

_Objects::iterator _Object_iter=this->_Myobjects.begin();
_Data::iterator _Data_iter=this->_Mydata.begin();

	for (_Objects::const_iterator _Iter=_Myobjects.begin(),
		_End=_Myobjects.end();_Iter!=_End;++_Iter) {
	
	const object &_Object=(*_Iter); //Increase speed
	
		//Worth doing
		if (this!=&_Object) {
		
		//Add objects
		this->_Myobjects.insert(_Object_iter,
			_Object._Myobjects.begin(),_Object._Myobjects.end());
		
		//Add data
		this->_Mydata.insert(_Data_iter,
			_Object._Mydata.begin(),_Object._Mydata.end());
		}
	}
}

//Object
//Public

script_class::object::object():_Depth(0),_Templated(false) {
	//Empty
}

script_class::object::object(const object &_Object) {
	copy(_Object);
}

script_class::object::~object() {
	//Empty
}

script_class::object &script_class::object::operator=(
	const object &_Object) {

	//Worth doing
	if (this!=&_Object) copy(_Object);

return *this;
}

bool script_class::object::operator==(const std::string &_Class_name) const {
	return string_utility::compare(this->_Class_name,_Class_name);
}

bool script_class::object::operator!=(const std::string &_Class_name) const {
	return !(*this==_Class_name);
}

const std::string &script_class::object::class_name() const {
	return _Class_name;
}

const std::string &script_class::object::name() const {
	return _Name;
}

const std::string &script_class::object::tag() const {
	return _Tag;
}

const std::string &script_class::object::parent() const {
	return _Parent;
}

unsigned int script_class::object::depth() const {
	return _Depth;
}

bool script_class::object::templated() const {
	return _Templated;
}

script_class::object::objects_iterator script_class::object::
	objects_begin() const {
	return _Myobjects.begin();
}

script_class::object::objects_iterator script_class::object::
	objects_end() const {
	return _Myobjects.end();
}

script_class::object::data_iterator script_class::object::
	data_begin() const {
	return _Mydata.begin();
}

script_class::object::data_iterator script_class::object::
	data_end() const {
	return _Mydata.end();
}

//Class
//Private

script_class::script_class(script_parser *_Owner,const std::string &_Name):
	_Owner(_Owner),_Name(_Name) {
	//Empty
}

script_class::script_class(const script_class &_Script_class):
	_Owner(0) {
	copy(_Script_class);
}

script_class::script_class(script_parser *_Owner,const std::string &_Name,
	const script_class &_Script_class):_Owner(_Owner),_Name(_Name) {
	copy(_Script_class);
}

script_class::~script_class() {
	clear_properties();
}

void script_class::copy(const script_class &_Script_class) {

	//A class only accepts scopes with equal owner
	if (_Owner==_Script_class._Owner) {
	
	_Myregistered_scopes=_Script_class._Myregistered_scopes;
	_Myregistered_inheritances=_Script_class._Myregistered_inheritances;
	}
}

//Public

script_class &script_class::operator=(const script_class &_Script_class) {

	//Worth doing
	if (this!=&_Script_class) copy(_Script_class);

return *this;
}

script_parser *script_class::owner() const {
	return _Owner;
}

const std::string &script_class::name() const {
	return _Name;
}

void script_class::tag(const std::string &_Tag) {
	this->_Tag=_Tag;
}

const std::string &script_class::tag() const {
	return _Tag;
}

//Properties

script_property *script_class::create_property(const std::string &_Name) {

	//Property already exist
	if (property_pointer(_Name)) return 0;

script_property *_Myproperty=new script_property(_Name);
_Myproperties.push_back(_Myproperty);
return _Myproperty;
}

script_property *script_class::create_property(const std::string &_Name,
	const script_property &_Script_property) {

	//Property already exist
	if (property_pointer(_Name)) return 0;

script_property *_Myproperty=new script_property(_Name,_Script_property);
_Myproperties.push_back(_Myproperty);
return _Myproperty;
}

void script_class::remove_property(script_property *&_Script_property) {

	for (_Properties::iterator _Iter=_Myproperties.begin(),
		_End=_Myproperties.end();_Iter!=_End;++_Iter) {
	
		if ((*_Iter)==_Script_property) {
		
		delete _Script_property;
		_Script_property=0; //Set referenced pointer to null
		
		_Myproperties.erase(_Iter);
		return;
		}
	}
}

void script_class::clear_properties() {

	for (_Properties::const_iterator _Iter=_Myproperties.begin(),
		_End=_Myproperties.end();_Iter!=_End;++_Iter)
			delete (*_Iter);

_Myproperties.clear();
}

script_class::properties_iterator script_class::properties_begin() const {
	return _Myproperties.begin();
}

script_class::properties_iterator script_class::properties_end() const {
	return _Myproperties.end();
}

script_property *script_class::property_pointer(const std::string &_Name) const {

	for (_Properties::const_iterator _Iter=_Myproperties.begin(),
		_End=_Myproperties.end();_Iter!=_End;++_Iter) {
	
		//Property name exist, return pointer
		if ((*_Iter)->_Name==_Name) return (*_Iter);
	}

return 0;
}

//Registration

void script_class::register_scope(script_scope *_Script_scope) {

	//A class only accepts scopes with equal owner
	if (owner()!=_Script_scope->owner()) return;
	
	for (_Scopes::const_iterator _Iter=_Myregistered_scopes.begin(),
		_End=_Myregistered_scopes.end();_Iter!=_End;++_Iter) {
	
		//No duplicates allowed
		if ((*_Iter)==_Script_scope) return;
	}

_Myregistered_scopes.push_back(_Script_scope);
}

void script_class::unregister_scope(script_scope *_Script_scope) {

	for (_Scopes::iterator _Iter=_Myregistered_scopes.begin(),
		_End=_Myregistered_scopes.end();_Iter!=_End;++_Iter) {
	
		if ((*_Iter)==_Script_scope) {
		
		_Myregistered_scopes.erase(_Iter); //Only erase node
		return;
		}
	}
}

void script_class::unregister_scopes() {
	_Myregistered_scopes.clear();
}

void script_class::register_inheritance(script_class *_Script_class) {

	//A class can only inherit from classes with equal owner
	if (owner()!=_Script_class->owner()) return;
	
	for (_Classes::const_iterator _Iter=_Myregistered_inheritances.begin(),
		_End=_Myregistered_inheritances.end();_Iter!=_End;++_Iter) {
	
		//No duplicates allowed
		if ((*_Iter)==_Script_class) return;
	}

_Myregistered_inheritances.push_back(_Script_class);
}

void script_class::unregister_inheritance(script_class *_Script_class) {

	for (_Classes::iterator _Iter=_Myregistered_inheritances.begin(),
		_End=_Myregistered_inheritances.end();_Iter!=_End;++_Iter) {
	
		if ((*_Iter)==_Script_class) {
		
		_Myregistered_inheritances.erase(_Iter); //Only erase node
		return;
		}
	}
}

void script_class::unregister_inheritances() {
	_Myregistered_inheritances.clear();
}

} //namespace lunar