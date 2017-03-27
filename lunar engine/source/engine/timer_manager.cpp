/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: timer_manager.cpp
---------------------------------------------------
*/

#include "timer_manager.h"

namespace lunar {

//Private

//This instance is allocated when core is initialized
//This instance is deallocated when core is destroyed
timer_manager *timer_manager::_Instance=0;

//Protected

timer_manager::timer_manager() {
	_Stopwatch.start();
}

timer_manager::timer_manager(const timer_manager &_Timer_manager):
	manager(_Timer_manager) {
	copy(_Timer_manager);
}

timer_manager::~timer_manager() {
	//Empty - let manager::~manager() do the clean up
}

timer_manager &timer_manager::operator=(const timer_manager &_Timer_manager) {

	//Worth doing
	if (this!=&_Timer_manager) {
	
	manager::operator=(_Timer_manager); //Invoke base operator=
	copy(_Timer_manager);
	}

return *this;
}

void timer_manager::copy(const timer_manager &_Timer_manager) {
	_Stopwatch=_Timer_manager._Stopwatch;
}

//Public

timer_manager *timer_manager::instance() {
	return _Instance;
}

void timer_manager::update(manageable *_Manageable) {
	//Empty - not in use
}

void timer_manager::update() {

float _Time=_Stopwatch.elapsed().interval; //Time since last update

timer *_Timer;

	for (_Manageables::const_iterator _Iter=_Mymanageables.begin(),
		_End=_Mymanageables.end();_Iter!=_End;++_Iter) {
	
	_Timer=static_cast<timer*>(*_Iter); //Increase speed
	
		//Make sure interval is valid
		if (!_Timer->_Enabled || _Timer->_Interval<=0.0f) continue;
		
		if ((_Timer->_Time+=_Time)>=_Timer->_Interval) {
		
		_Timer->_Time=0.0f;
		_Timer->_Ticked=true;
		
			//Check for callback
			if (_Timer->_Ontick) (*_Timer->_Ontick)(_Timer); 
		}
	}
}

void timer_manager::sync() {
	_Stopwatch.elapsed();
}

//Timer functions

timer *timer_manager::create_timer(float _Interval,bool _Enabled,
	bool _Secured) {

timer *_Mytimer=new timer(this,_Interval,_Enabled);
add(_Mytimer,_Secured);
return _Mytimer;
}

timer *timer_manager::create_timer(const timer &_Timer,bool _Secured) {

timer *_Mytimer=new timer(this,_Timer);
add(_Mytimer,_Secured);
return _Mytimer;
}

} //namespace lunar