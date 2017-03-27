/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: script_scope.cpp
---------------------------------------------------
*/

#include "script_scope.h"

//Dependencies
#include "script_class.h"

namespace lunar {

//Private

script_scope::script_scope(script_parser *_Owner,const std::string &_Name):
	_Owner(_Owner),_Name(_Name) {
	//Empty
}

script_scope::script_scope(const script_scope &_Script_scope):
	_Owner(0) {
	copy(_Script_scope);
}

script_scope::script_scope(script_parser *_Owner,const std::string &_Name,
	const script_scope &_Script_scope):_Owner(_Owner),_Name(_Name) {
	copy(_Script_scope);
}

script_scope::~script_scope() {
	//Empty
}

void script_scope::copy(const script_scope &_Script_scope) {

	//A scope only accepts classes with equal owner
	if (_Owner==_Script_scope._Owner)
		_Myregistered_classes=_Script_scope._Myregistered_classes;
}

//Public

script_scope &script_scope::operator=(const script_scope &_Script_scope) {

	//Worth doing
	if (this!=&_Script_scope) copy(_Script_scope);

return *this;
}

script_parser *script_scope::owner() const {
	return _Owner;
}

const std::string &script_scope::name() const {
	return _Name;
}

//Registration

void script_scope::register_class(script_class *_Script_class) {

	//A scope only accepts classes with equal owner
	if (_Owner!=_Script_class->owner()) return;
	
	for (_Classes::const_iterator _Iter=_Myregistered_classes.begin(),
		_End=_Myregistered_classes.end();_Iter!=_End;++_Iter) {
	
		//No duplicates allowed
		if ((*_Iter)==_Script_class) return;
	}

_Myregistered_classes.push_back(_Script_class);
}

void script_scope::unregister_class(script_class *_Script_class) {

	for (_Classes::iterator _Iter=_Myregistered_classes.begin(),
		_End=_Myregistered_classes.end();_Iter!=_End;++_Iter) {
	
		if ((*_Iter)==_Script_class) {
		
		_Myregistered_classes.erase(_Iter); //Only erase node
		return;
		}
	}
}

void script_scope::unregister_classes() {
	_Myregistered_classes.clear();
}

} //namespace lunar