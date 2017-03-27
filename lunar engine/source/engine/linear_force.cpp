/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: linear_force.cpp
---------------------------------------------------
*/

#include "linear_force.h"

//Dependencies
#include "particle.h"

namespace lunar {

//Protected

linear_force::linear_force():_Type(ADD) {
	//Empty
}

linear_force::linear_force(const linear_force &_Linear_force):
	affector(_Linear_force) {
	copy(_Linear_force);
}

linear_force::~linear_force() {
	//Empty
}

void linear_force::copy(const linear_force &_Linear_force) {

_Type=_Linear_force._Type;
_Force=_Linear_force._Force;
}

void linear_force::affect(particle *_Particle,particle *_Particle_end,
	float _Time) {

	if (!_Enabled) return;

vector2 _Scaled_force(_Force*_Time);

	for (;_Particle!=_Particle_end;++_Particle) {
	
		if (_Type==ADD) _Particle->_Direction+=_Scaled_force;
		else _Particle->_Direction=(_Particle->_Direction+_Force)*0.5f;
	}
}

//Public

linear_force &linear_force::operator=(const linear_force &_Linear_force) {

	//Worth doing
	if (this!=&_Linear_force) {
	
	affector::operator=(_Linear_force); //Invoke base operator=
	copy(_Linear_force);
	}

return *this;
}

void linear_force::type(const TYPE &_Type) {
	this->_Type=_Type;
}

linear_force::TYPE linear_force::type() const {
	return _Type;
}

void linear_force::force(const vector2 &_Force) {
	this->_Force=_Force;
}

const vector2 &linear_force::force() const {
	return _Force;
}

} //namespace lunar