/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: sound_table.cpp
---------------------------------------------------
*/

#include "sound_table.h"

//Dependencies
#include "sound_container.h"
#include "connector.h"
#include "any_cast.h"

namespace lunar {

//Private

//This instance is allocated when resource_manager is initialized
//This instance is deallocated when resource_manager is destroyed
sound_table *sound_table::_Instance=0;

//Protected

void sound_table::copy(const sound_table &_Sound_table) {

_Stream_column=_Sound_table._Stream_column;
_Software_column=_Sound_table._Software_column;
_Positional_column=_Sound_table._Positional_column;
_Loop_column=_Sound_table._Loop_column;

_Sound_container=_Sound_table._Sound_container;
}

//Public

sound_table::sound_table(sound_container *_Sound_container):
	file_table("sound"),_Stream_column("stream"),_Software_column("software"),
		_Positional_column("positional"),_Loop_column("loop"),
			_Sound_container(_Sound_container) {
	//Empty
}

sound_table::sound_table(const sound_table &_Sound_table):
	file_table(_Sound_table) {
	copy(_Sound_table);
}

sound_table::~sound_table() {
	//Empty
}

sound_table &sound_table::operator=(const sound_table &_Sound_table) {

	//Worth doing
	if (this!=&_Sound_table) {
	
	file_table::operator=(_Sound_table); //Invoke base operator=
	copy(_Sound_table);
	}

return *this;
}

sound_table *sound_table::instance() {
	return _Instance;
}

bool sound_table::load(connector *_Connector) {

//Main query (sound)
std::string _Query("SELECT `"+_Name_column+"`,`"+_Filename_column+"`,`"+
	_Stream_column+"`,`"+_Software_column+"`,`"+_Positional_column+"`,`"+
		_Loop_column+"` FROM `"+_Name+"`");

connector::query_data _Data;

	if (!_Connector->query(_Query,_Data)) return false;
	if (_Data.empty()) return true;

_Sound_container->increase_reserve(_Data.size()); //Vector optimization

	for (connector::query_iter _Iter=_Data.begin(),
		_End=_Data.end();_Iter!=_End;++_Iter) {
	
	connector::row_iter _Row_iter=(*_Iter).begin();
	
	std::string _Name=(*_Row_iter);
	std::string _Filename=*(++_Row_iter);
	bool _Stream=any_cast<bool>(*(++_Row_iter));
	bool _Software=any_cast<bool>(*(++_Row_iter));
	bool _Positional=any_cast<bool>(*(++_Row_iter));
	bool _Loop=any_cast<bool>(*(++_Row_iter));
	
	_Sound_container->add_data(sound_data(_Name,_Filename,
		_Stream,_Software,_Positional,_Loop));
	}

return true;
}

void sound_table::columns(const std::string &_Stream_column,
	const std::string &_Software_column,const std::string &_Positional_column,
		const std::string &_Loop_column) {

this->_Stream_column=_Stream_column;
this->_Software_column=_Software_column;
this->_Positional_column=_Positional_column;
this->_Loop_column=_Loop_column;
}

const std::string &sound_table::stream_column() const {
	return _Stream_column;
}

const std::string &sound_table::software_column() const {
	return _Software_column;
}

const std::string &sound_table::positional_column() const {
	return _Positional_column;
}

const std::string &sound_table::loop_column() const {
	return _Loop_column;
}

} //namespace lunar