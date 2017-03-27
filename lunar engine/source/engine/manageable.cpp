/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: manageable.cpp
---------------------------------------------------
*/

#include "manageable.h"

//Dependencies
#include "manager.h"

namespace lunar {

//Protected

manageable::manageable(manager *_Owner,resource_data *_Data):_Owner(_Owner),
	_Status(NEED_UPDATE),_Data(_Data),_Recent_data(0) {
	//Empty
}

manageable::manageable(const manageable &_Manageable):_Owner(0),
	_Status(NEED_UPDATE) {

_Recent_data=0; //No previously stored data
copy(_Manageable);
}

manageable::manageable(manager *_Owner,const manageable &_Manageable):
	_Owner(_Owner),_Status(NEED_UPDATE) {

_Recent_data=0; //No previously stored data
copy(_Manageable);
}

manageable::~manageable() {

	if (!_Mylisteners.empty()) {
	
	manageable_listener *_Manageable_listener;
	
		for (_Listeners::const_iterator _Iter=_Mylisteners.begin(),
			_End=_Mylisteners.end();_Iter!=_End;++_Iter) {
		
		_Manageable_listener=(*_Iter); //Increase speed
		
			//Notify removed to subscribers
			if (_Manageable_listener->listening())
				_Manageable_listener->manageable_removed(this);
		}
	}
}

manageable &manageable::operator=(const manageable &_Manageable) {

	//Worth doing
	if (this!=&_Manageable) {
	
	bool _Changed=(_Data!=_Manageable._Data);
	
		if (_Changed) _Recent_data=_Data; //Store previous data
	
	copy(_Manageable);
	
		if (_Changed) changed();
	}

return *this;
}

void manageable::copy(const manageable &_Manageable) {
	_Data=_Manageable._Data;
}

void manageable::changed() {

status(NEED_UPDATE); //Update me

	if (!_Mylisteners.empty()) {
	
	manageable_listener *_Manageable_listener;
	
		for (_Listeners::const_iterator _Iter=_Mylisteners.begin(),
			_End=_Mylisteners.end();_Iter!=_End;++_Iter) {
		
		_Manageable_listener=(*_Iter); //Increase speed
		
			//Notify changed to subscribers
			if (_Manageable_listener->listening())
				_Manageable_listener->manageable_changed(this);
		}
	}
}

void manageable::status_changed() {

	if (!_Mylisteners.empty()) {
	
	manageable_listener *_Manageable_listener;
	
		for (_Listeners::const_iterator _Iter=_Mylisteners.begin(),
			_End=_Mylisteners.end();_Iter!=_End;++_Iter) {
		
		_Manageable_listener=(*_Iter); //Increase speed
		
			//Notify status changed to subscribers
			if (_Manageable_listener->listening())
				_Manageable_listener->manageable_statuschanged(this);
		}
	}
}

void manageable::status(const STATUS &_Status) {

	if (this->_Status!=_Status) {
	
	this->_Status=_Status;
	status_changed();
	}
}

void manageable::data(resource_data *_Data) {

	if (this->_Data!=_Data) {
	
	_Recent_data=this->_Data; //Store previous data
	this->_Data=_Data;
	changed();
	}
}

//Public

bool manageable::operator==(const manageable &_Manageable) const {
	return (_Data==_Manageable._Data);
}

bool manageable::operator!=(const manageable &_Manageable) const {
	return !(*this==_Manageable);
}

void manageable::update() {

	if (_Owner) _Owner->update(this);
}

void manageable::refresh() {

	//Only refresh if previous update failed
	if (_Status==FAILED) status(NEED_UPDATE); //Update me
}

manager *manageable::owner() const {
	return _Owner;
}

manageable::STATUS manageable::status() const {
	return _Status;
}

resource_data *manageable::data() const {
	return _Data;
}

resource_data *manageable::recent_data() const {
	return _Recent_data;
}

} //namespace lunar