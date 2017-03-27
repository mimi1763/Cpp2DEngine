/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: thread.cpp
---------------------------------------------------
*/

#include "thread.h"

//Dependencies
#include "thread_manager.h"

namespace lunar {

//Protected

thread::thread(thread_manager *_Owner,entry_point _Entry_point,
	void *_Argument):manageable(_Owner,0),_Pthread(0),_Entry_point(
		_Entry_point),_Argument(_Argument),_Join(true) {
	//Empty
}

thread::thread(const thread &_Thread):manageable(_Thread) {

_Pthread=0; //Request pthread
copy(_Thread);
}

thread::thread(thread_manager *_Owner,const thread &_Thread):
	manageable(_Owner,_Thread) {

_Pthread=0; //Request pthread
copy(_Thread);
}

thread::~thread() {

	if (_Pthread) {
	
	stop();
	delete _Pthread;
	}
}

void thread::copy(const thread &_Thread) {

_Entry_point=_Thread._Entry_point;
_Argument=_Thread._Argument;
_Join=_Thread._Join;
}

//Public

thread &thread::operator=(const thread &_Thread) {

	//Worth doing
	if (this!=&_Thread) {
	
	manageable::operator=(_Thread); //Invoke base operator=
	
	bool _Changed=(_Entry_point!=_Thread._Entry_point);
	copy(_Thread);
	
		//Update explicit
		if (_Changed) changed();
	}

return *this;
}

bool thread::operator==(const thread &_Thread) const {
	return (_Entry_point==_Thread._Entry_point); //Redefine equality
}

bool thread::operator!=(const thread &_Thread) const {
	return !(*this==_Thread); //Redefine inequality
}

bool thread::stop() {

	if (!_Pthread) return true;
	
	//Wait for thread to complete/join with main
	if (_Join && pthread_join(*_Pthread,0)==0) return true;

//Cancel the thread immediately
return (pthread_cancel(*_Pthread)==0);
}

void thread::entrypoint(entry_point _Entry_point) {

	if (this->_Entry_point!=_Entry_point) {
	
	this->_Entry_point=_Entry_point;
	changed(); //Update explicit
	}
}

thread::entry_point thread::entrypoint() const {
	return _Entry_point;
}

void thread::argument(void *_Argument) {
	this->_Argument=_Argument;
}

void *thread::argument() const {
	return _Argument;
}

void thread::join(bool _Join) {
	this->_Join=_Join;
}

bool thread::join() const {
	return _Join;
}

} //namespace lunar