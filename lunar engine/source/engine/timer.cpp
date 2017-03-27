/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: timer.cpp
---------------------------------------------------
*/

#include "timer.h"

//Dependencies
#include "timer_manager.h"

namespace lunar {

//Protected

timer::timer(timer_manager *_Owner,float _Interval,bool _Enabled):
	manageable(_Owner,0),_Interval(_Interval),_Time(0.0f),_Enabled(_Enabled),
		_Ticked(false),_Ontick(0) {
	//Empty
}

timer::timer(const timer &_Timer):manageable(_Timer) {

_Ontick=0; //No callback
copy(_Timer);
}

timer::timer(timer_manager *_Owner,const timer &_Timer):
	manageable(_Owner,_Timer) {

_Ontick=0; //No callback
copy(_Timer);
}

timer::~timer() {
	release_ontick();
}

void timer::copy(const timer &_Timer) {

_Interval=_Timer._Interval;
_Time=_Timer._Time;
_Enabled=_Timer._Enabled;
_Ticked=_Timer._Ticked;
}

//Public

timer &timer::operator=(const timer &_Timer) {

	//Worth doing
	if (this!=&_Timer) {
	
	manageable::operator=(_Timer); //Invoke base operator=
	
	bool _Changed=(_Interval!=_Timer._Interval);
	copy(_Timer);
	
		//Call in case someone is listening
		if (_Changed) changed();
	}

return *this;
}

bool timer::operator==(const timer &_Timer) const {
	return (_Interval==_Timer._Interval); //Redefine equality
}

bool timer::operator!=(const timer &_Timer) const {
	return !(*this==_Timer); //Redefine inequality
}

void timer::start() {
	_Enabled=true;
}

void timer::stop(bool _Reset) {

_Enabled=false;

	if (_Reset) {
	
	_Time=0;
	_Ticked=false;
	}
}

void timer::interval(float _Interval) {

	if (this->_Interval!=_Interval) {
	
	this->_Interval=_Interval;
	changed(); //Call in case someone is listening
	}
}

float timer::interval() const {
	return _Interval;
}

float timer::time() const {
	return _Time;
}

bool timer::enabled() const {
	return _Enabled;
}

bool timer::ticked() {

	if (_Ticked) {
	
	_Ticked=false;
	return true;
	}

return false;
}

//Callbacks

void timer::ontick(static_callback<timer*>::function _Function) {

	if (_Ontick) release_ontick();

_Ontick=new static_callback<timer*>(_Function);
}

void timer::release_ontick() {

	if (_Ontick) {
	
	delete _Ontick;
	_Ontick=0;
	}
}

} //namespace lunar