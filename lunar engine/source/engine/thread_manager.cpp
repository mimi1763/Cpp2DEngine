/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: thread_manager.cpp
---------------------------------------------------
*/

#include "thread_manager.h"

namespace lunar {

//Private

//This instance is allocated when core is initialized
//This instance is deallocated when core is destroyed
thread_manager *thread_manager::_Instance=0;

//Protected

void thread_manager::copy(const thread_manager &_Thread_manager) {
	//Empty - override default memcpy behaviour
}

//Pthread specific

bool thread_manager::init_pthread() {

	//Init thread attribute
	if (pthread_attr_init(&_Thread_attribute)!=0) {
	
	_Thread_attribute=0;
	return false;
	}
	
	//Init thread mutex
	if (pthread_mutex_init(&_Thread_mutex,0)!=0) {
	
	_Thread_mutex=0;
	return false;
	}

//Set thread attribute as joinable
return (pthread_attr_setdetachstate(&_Thread_attribute,PTHREAD_CREATE_JOINABLE)==0); 
}

bool thread_manager::init_thread(thread *_Thread) const {
	return (pthread_create(_Thread->_Pthread,&_Thread_attribute,
		_Thread->_Entry_point,_Thread->_Argument)==0);
}

//Public

thread_manager::thread_manager() {
	_Supported=init_pthread();
}

thread_manager::thread_manager(const thread_manager &_Thread_manager):
	manager(_Thread_manager) {

_Supported=init_pthread();
copy(_Thread_manager);
}

thread_manager::~thread_manager() {

tidy(); //Call before destroying attribute and mutex

	//Destroy thread attribute
	if (_Thread_attribute) pthread_attr_destroy(&_Thread_attribute);
	
	//Destroy thread mutex
	if (_Thread_mutex) pthread_mutex_destroy(&_Thread_mutex);
}

thread_manager &thread_manager::operator=(const thread_manager &_Thread_manager) {

	//Worth doing
	if (this!=&_Thread_manager) {
	
	manager::operator=(_Thread_manager); //Invoke base operator=
	copy(_Thread_manager);
	}

return *this;
}

thread_manager *thread_manager::instance() {
	return _Instance;
}

void thread_manager::update(manageable *_Manageable) {

	//No need for update
	if (_Manageable->status()!=manageable::NEED_UPDATE) return;

thread *_Thread=dynamic_cast<thread*>(_Manageable);

	//Invalid type for this manager
	if (!_Thread) return;
	
	//Not supported
	if (!_Supported) {
	
	status(_Thread,manageable::FAILED);
	return;
	}
	
	//Create pthread
	if (!_Thread->_Pthread) _Thread->_Pthread=new pthread_t();
	else {
	
		//Try to stop the old thread
		if (!_Thread->stop()) {
		
		status(_Thread,manageable::FAILED);
		return;
		}
	}

status(_Thread,init_thread(_Thread)?manageable::OKAY:manageable::FAILED); //Pending
}

void thread_manager::update() {
	manager::update(); //Must redirect because of name reuse
}

bool thread_manager::mutex_lock() {
	return (pthread_mutex_lock(&_Thread_mutex)==0);
}

bool thread_manager::mutex_trylock() {
	return (pthread_mutex_trylock(&_Thread_mutex)==0);
}

bool thread_manager::mutex_unlock() {
	return (pthread_mutex_unlock(&_Thread_mutex)==0);
}

//Thread functions

thread *thread_manager::create_thread(thread::entry_point _Entry_point,
	void *_Argument,bool _Secured) {

thread *_Mythread=new thread(this,_Entry_point,_Argument);
add(_Mythread,_Secured);
return _Mythread;
}

thread *thread_manager::create_thread(const thread &_Thread,bool _Secured) {

thread *_Mythread=new thread(this,_Thread);
add(_Mythread,_Secured);
return _Mythread;
}

} //namespace lunar