/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: direction_randomizer.cpp
---------------------------------------------------
*/

#include "direction_randomizer.h"

//Dependencies
#include "particle.h"
#include "math_utility.h"

namespace lunar {

//Protected

direction_randomizer::direction_randomizer():_Randomness(0.0f),
	_Scope(1.0f),_Keep_velocity(false) {
	//Empty
}

direction_randomizer::direction_randomizer(const direction_randomizer &_Direction_randomizer):
	affector(_Direction_randomizer) {
	copy(_Direction_randomizer);
}

direction_randomizer::~direction_randomizer() {
	//Empty
}

void direction_randomizer::copy(const direction_randomizer &_Direction_randomizer) {

_Randomness=_Direction_randomizer._Randomness;
_Scope=_Direction_randomizer._Scope;
_Keep_velocity=_Direction_randomizer._Keep_velocity;
}

void direction_randomizer::affect(particle *_Particle,particle *_Particle_end,
	float _Time) {

	if (!_Enabled) return;

float _Length=0.0f;
float _New_length=0.0f;

	for (;_Particle!=_Particle_end;++_Particle) {
	
		if (_Scope>math::random()) {
		
			if (_Keep_velocity) _Length=_Particle->_Direction.length();
		
		_Particle->_Direction+=vector2(math::random(-_Randomness,
			_Randomness)*_Time,math::random(-_Randomness,_Randomness)*_Time);
		
			if (_Keep_velocity) {
			
			_New_length=_Particle->_Direction.length();
			
				if (_New_length!=0.0f) _Particle->_Direction*=_Length/
					_New_length;
			}
		}
	}
}

//Public

direction_randomizer &direction_randomizer::operator=(
	const direction_randomizer &_Direction_randomizer) {

	//Worth doing
	if (this!=&_Direction_randomizer) {
	
	affector::operator=(_Direction_randomizer); //Invoke base operator=
	copy(_Direction_randomizer);
	}

return *this;
}

void direction_randomizer::randomness(float _Randomness) {
	this->_Randomness=_Randomness;
}

float direction_randomizer::randomness() const {
	return _Randomness;
}

void direction_randomizer::scope(float _Scope) {

	if (_Scope>1.0f) this->_Scope=1.0f;
	else if (_Scope<0.0f) this->_Scope=0.0f;
	else this->_Scope=_Scope;
}

float direction_randomizer::scope() const {
	return _Scope;
}

void direction_randomizer::keep_velocity(bool _Keep_velocity) {
	this->_Keep_velocity=_Keep_velocity;
}

bool direction_randomizer::keep_velocity() const {
	return _Keep_velocity;
}

} //namespace lunar