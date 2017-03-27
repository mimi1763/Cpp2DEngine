/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: sine_force.cpp
---------------------------------------------------
*/

#include "sine_force.h"

//Dependencies
#include "particle.h"
#include "math_utility.h"

namespace lunar {

//Protected

sine_force::sine_force():_Type(ADD),_Frequency(1.0f),
	_Frequency_min(1.0f),_Frequency_max(1.0f),_Angle(0.0f) {
	//Empty
}

sine_force::sine_force(const sine_force &_Sine_force):
	affector(_Sine_force) {
	copy(_Sine_force);
}

sine_force::~sine_force() {
	//Empty
}

void sine_force::copy(const sine_force &_Sine_force) {

_Type=_Sine_force._Type;
_Force=_Sine_force._Force;

_Frequency=_Sine_force._Frequency;
_Frequency_min=_Sine_force._Frequency_min;
_Frequency_max=_Sine_force._Frequency_max;
_Angle=_Sine_force._Angle;
}

void sine_force::affect(particle *_Particle,particle *_Particle_end,
	float _Time) {

	if (!_Enabled) return;

_Angle+=_Frequency*_Time;
vector2 _Scaled_force(_Force*(math::sine(_Angle)*_Time));

	if (_Angle>math::TWO_PI) {
	
	_Angle=0.0f; //Reset angle
	_Frequency=math::random(_Frequency_min,_Frequency_max); //Change frequency
	}
	
	for (;_Particle!=_Particle_end;++_Particle) {
	
		if (_Type==ADD) _Particle->_Direction+=_Scaled_force;
		else _Particle->_Direction=(_Particle->_Direction+_Force)*0.5f;
	}
}

//Public

sine_force &sine_force::operator=(const sine_force &_Sine_force) {

	//Worth doing
	if (this!=&_Sine_force) {
	
	affector::operator=(_Sine_force); //Invoke base operator=
	copy(_Sine_force);
	}

return *this;
}

void sine_force::type(const TYPE &_Type) {
	this->_Type=_Type;
}

sine_force::TYPE sine_force::type() const {
	return _Type;
}

void sine_force::force(const vector2 &_Force) {
	this->_Force=_Force;
}

const vector2 &sine_force::force() const {
	return _Force;
}

void sine_force::frequency(float _Frequency_min,
	float _Frequency_max) {

this->_Frequency_min=_Frequency_min;
this->_Frequency_max=(_Frequency_max<_Frequency_min?
	_Frequency_min:_Frequency_max);

_Frequency=math::random(this->_Frequency_min,this->_Frequency_max);
}

float sine_force::frequency_min() const {
	return _Frequency_min;
}

float sine_force::frequency_max() const {
	return _Frequency_max;
}

} //namespace lunar