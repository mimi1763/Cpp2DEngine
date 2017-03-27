/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: scaler.cpp
---------------------------------------------------
*/

#include "scaler.h"

//Dependencies
#include "particle.h"
#include "math_utility.h"

namespace lunar {

//Protected

scaler::scaler() {
	//Empty
}

scaler::scaler(const scaler &_Scaler):affector(_Scaler) {
	copy(_Scaler);
}

scaler::~scaler() {
	//Empty
}

void scaler::copy(const scaler &_Scaler) {
	_Myscale_points=_Scaler._Myscale_points;
}

void scaler::affect(particle *_Particle,particle *_Particle_end,
	float _Time) {

	if (!_Enabled || _Myscale_points.size()<1) return;

const size_t _Size=_Myscale_points.size();

	for (;_Particle!=_Particle_end;++_Particle) {
	
	float _Particle_time=1.0f-(_Particle->_Lifetime/_Particle->_Total_lifetime);
	
		if (_Size==1 && _Particle_time>=_Myscale_points[0]._Percent) {
		
		_Particle->_Size=_Myscale_points[0]._Size;
		continue;
		}
		
		for (size_t i=0;i<_Size-1;++i) {
		
		float &_Time_from=_Myscale_points[i]._Percent;
		float &_Time_to=_Myscale_points[i+1]._Percent;
		
			if (_Particle_time>=_Time_from && _Particle_time<_Time_to) {
			
			//Convert particle time from absolute to relative
			_Particle_time-=_Time_from;
			_Particle_time/=_Time_to-_Time_from;
			
			_Particle->_Size=math::lerp(_Myscale_points[i]._Size,
				_Myscale_points[i+1]._Size,_Particle_time);
			break;
			}
		}
	}
}

//Public

scaler &scaler::operator=(const scaler &_Scaler) {

	//Worth doing
	if (this!=&_Scaler) {
	
	affector::operator=(_Scaler); //Invoke base operator=
	copy(_Scaler);
	}

return *this;
}

void scaler::add_scalepoint(float _Size,float _Percent) {

_Scale_point _Myscale_point;
_Myscale_point._Size=_Size;
_Myscale_point._Percent=_Percent;
_Myscale_points.push_back(_Myscale_point);
}

void scaler::change_scalepoint(const size_t &_Index,float _Size,
	float _Percent) {

	if (_Index<_Myscale_points.size()) {
	
	_Myscale_points[_Index]._Size=_Size;
	_Myscale_points[_Index]._Percent=_Percent;
	}
}

void scaler::remove_scalepoint(const size_t &_Index) {

	if (_Index<_Myscale_points.size()) _Myscale_points.erase(
		_Myscale_points.begin()+_Index);
}

void scaler::clear_scalepoints() {
	_Myscale_points.clear();
}

} //namespace lunar