/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: gravitation.cpp
---------------------------------------------------
*/

#include "gravitation.h"

//Dependencies
#include "particle.h"

namespace lunar {

//Protected

gravitation::gravitation():_Gravity(100.0f),_Mass(100.0f) {
	//Empty
}

gravitation::gravitation(const gravitation &_Gravitation):
	affector(_Gravitation) {
	copy(_Gravitation);
}

gravitation::~gravitation() {
	//Empty
}

void gravitation::copy(const gravitation &_Gravitation) {

_Position=_Gravitation._Position;
_Gravity=_Gravitation._Gravity;
_Mass=_Gravitation._Mass;
}

void gravitation::affect(particle *_Particle,particle *_Particle_end,
	float _Time) {

	if (!_Enabled) return;
	
	for (;_Particle!=_Particle_end;++_Particle) {
	
	//Newton's law of universal gravitation
	vector2 _Distance(_Position-_Particle->_Position);
	float _Length=_Distance.squared_length();
	
		if (_Length>0.0f)
		{
			float _Force=(_Gravity*_Mass*_Particle->_Mass)/_Length;
			_Particle->_Direction+=_Distance*_Force*_Time;
		}
	}	
}

//Public

gravitation &gravitation::operator=(const gravitation &_Gravitation) {

	//Worth doing
	if (this!=&_Gravitation) {
	
	affector::operator=(_Gravitation); //Invoke base operator=
	copy(_Gravitation);
	}

return *this;
}

void gravitation::position(const vector2 &_Position) {
	this->_Position=_Position;
}

const vector2 &gravitation::position() const {
	return _Position;
}

void gravitation::gravity(float _Gravity) {
	this->_Gravity=_Gravity;
}

float gravitation::gravity() const {
	return _Gravity;
}

void gravitation::mass(float _Mass) {
	this->_Mass=_Mass;
}

float gravitation::mass() const {
	return _Mass;
}

} //namespace lunar