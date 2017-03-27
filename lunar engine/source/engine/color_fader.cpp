/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: color_fader.cpp
---------------------------------------------------
*/

#include "color_fader.h"

//Dependencies
#include "particle.h"

namespace lunar {

//Protected

color_fader::color_fader() {
	//Empty
}

color_fader::color_fader(const color_fader &_Color_fader):
	affector(_Color_fader) {
	copy(_Color_fader);
}

color_fader::~color_fader() {
	//Empty
}

void color_fader::copy(const color_fader &_Color_fader) {
	_Myfade_colors=_Color_fader._Myfade_colors;
}

void color_fader::affect(particle *_Particle,particle *_Particle_end,
	float _Time) {

	if (!_Enabled || _Myfade_colors.size()<1) return;

const size_t _Size=_Myfade_colors.size();

	for (;_Particle!=_Particle_end;++_Particle) {
	
	float _Particle_time=1.0f-(_Particle->_Lifetime/_Particle->_Total_lifetime);
	
		if (_Size==1 && _Particle_time>=_Myfade_colors[0]._Percent) {
		
		_Particle->_Color=_Myfade_colors[0]._Color;
		continue;
		}
		
		for (size_t i=0;i<_Size-1;++i) {
		
		float &_Time_from=_Myfade_colors[i]._Percent;
		float &_Time_to=_Myfade_colors[i+1]._Percent;
		
			if (_Particle_time>=_Time_from && _Particle_time<_Time_to) {
			
			//Convert particle time from absolute to relative
			_Particle_time-=_Time_from;
			_Particle_time/=_Time_to-_Time_from;
			
			_Particle->_Color=_Myfade_colors[i]._Color.mix(
				_Myfade_colors[i+1]._Color,_Particle_time);
			break;
			}
		}
	}
}

//Public

color_fader &color_fader::operator=(const color_fader &_Color_fader) {

	//Worth doing
	if (this!=&_Color_fader) {
	
	affector::operator=(_Color_fader); //Invoke base operator=
	copy(_Color_fader);
	}

return *this;
}

void color_fader::add_fadecolor(const color &_Color,float _Percent) {

_Fade_color _Myfade_color;
_Myfade_color._Color=_Color;
_Myfade_color._Percent=_Percent;
_Myfade_colors.push_back(_Myfade_color);
}

void color_fader::change_fadecolor(const size_t &_Index,const color &_Color,
	float _Percent) {

	if (_Index<_Myfade_colors.size()) {
	
	_Myfade_colors[_Index]._Color=_Color;
	_Myfade_colors[_Index]._Percent=_Percent;
	}
}

void color_fader::remove_fadecolor(const size_t &_Index) {

	if (_Index<_Myfade_colors.size()) _Myfade_colors.erase(
		_Myfade_colors.begin()+_Index);
}

void color_fader::clear_fadecolors() {
	_Myfade_colors.clear();
}

} //namespace lunar