/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: timeline.cpp
---------------------------------------------------
*/

#include "timeline.h"

//Dependencies
#include "system.h"

namespace lunar {

//Stopwatch
//Private

void stopwatch::copy(const stopwatch &_Stopwatch) {

_Time=_Stopwatch._Time;
_Enabled=_Stopwatch._Enabled;

_Ticks=_Stopwatch._Ticks;
}

//Public

stopwatch::stopwatch():_Enabled(false),_Ticks(0) {

_Time.total=0.0f;
_Time.interval=0.0f;
}

stopwatch::stopwatch(const stopwatch &_Stopwatch) {
	copy(_Stopwatch);
}

stopwatch::~stopwatch() {
	//Empty
}

stopwatch &stopwatch::operator=(const stopwatch &_Stopwatch) {

	//Worth doing
	if (this!=&_Stopwatch) copy(_Stopwatch);

return *this;
}

void stopwatch::start() {

	if (!_Enabled) {
	
	_Ticks=timeline::ticks();
	_Enabled=true;
	}
}

void stopwatch::stop(bool _Reset) {

	if (_Reset) {
	
	_Time.total=0.0f;
	_Time.interval=0.0f;
	}
	//Sync to this point
	else elapsed();

_Enabled=false;
}

const stopwatch::time &stopwatch::elapsed() {

	if (_Enabled) {
	
	long long _Current_ticks=timeline::ticks();
	_Time.total+=_Time.interval=timeline::
		elapsed(_Ticks,_Current_ticks); //Update time
	_Ticks=_Current_ticks;
	}

return _Time;
}

bool stopwatch::enabled() const {
	return _Enabled;
}

//Timeline
//Private

stopwatch timeline::_Stopwatch;
long long timeline::_Frequency=0;

timeline::timeline() {
	reset();
}

timeline::~timeline() {
	//Empty
}

void timeline::update(timer *_Timer) {
	_Stopwatch.elapsed(); //Update elapsed
}

void timeline::frequency(timer *_Timer) {
	_Frequency=system::cpu_frequency();
}

//Public

void timeline::reset() {

frequency(0); //Update frequency manually
_Stopwatch.stop();
_Stopwatch.start();
}

float timeline::elapsed() {
	return _Stopwatch.elapsed().total;
}

bool timeline::high_resolution() {
	return (_Frequency>0);
}

//Utility functions

long long timeline::ticks() {
	return (_Frequency>0?system::cpu_ticks():system::ticks());
}

float timeline::elapsed(long long _From_ticks,
	long long _To_ticks) {
	return (_Frequency>0?(float)(_To_ticks-_From_ticks)/_Frequency:
		(float)((unsigned int)_To_ticks-(unsigned int)_From_ticks)/1000);
}

} //namespace lunar