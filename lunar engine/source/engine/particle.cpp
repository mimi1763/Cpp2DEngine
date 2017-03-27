/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: particle.cpp
---------------------------------------------------
*/

#include "particle.h"

namespace lunar {

//Private

particle::particle():_Size(0),_Lifetime(0),_Mass(0),_Total_lifetime(0) {
	//Empty
}

particle::particle(const vector2 &_Position,const vector2 &_Direction,
	float _Size,float _Lifetime,float _Mass,
		const color &_Color):_Position(_Position),_Direction(_Direction),
			_Size(_Size),_Lifetime(_Lifetime),_Mass(_Mass),_Color(_Color),
				_Total_lifetime(_Lifetime) {
	//Empty
}

particle::particle(const particle &_Particle) {
	copy(_Particle);
}

particle::~particle() {
	//Empty
}

void particle::copy(const particle &_Particle) {

_Position=_Particle._Position;
_Direction=_Particle._Direction;
_Size=_Particle._Size;

_Lifetime=_Particle._Lifetime;
_Mass=_Particle._Mass;
_Color=_Particle._Color;

_Total_lifetime=_Particle._Total_lifetime;
}

//Public

particle &particle::operator=(const particle &_Particle) {

	//Worth doing
	if (this!=&_Particle) copy(_Particle);

return *this;
}

} //namespace lunar