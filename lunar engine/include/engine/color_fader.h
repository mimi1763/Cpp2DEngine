/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: color_fader.h
---------------------------------------------------
*/

#ifndef _COLOR_FADER_
#define _COLOR_FADER_

#include <vector>

#include "affector.h"
#include "color.h"

namespace lunar {

class color_fader:public affector {

	friend affector_manager;

protected:

	struct _Fade_color {
	
	color _Color;
	float _Percent;
	};
	
	typedef std::vector<_Fade_color> _Fade_colors; //Need random access
	
	_Fade_colors _Myfade_colors;
	
	//Protected constructor
	color_fader();
	
	//Protected copy constructor
	color_fader(const color_fader &_Color_fader);
	
	//Protected virtual destructor
	virtual ~color_fader();
	
	void copy(const color_fader &_Color_fader);
	
	virtual void affect(particle *_Particle,particle *_Particle_end,
		float _Time);

public:

	color_fader &operator=(const color_fader &_Color_fader);
	
	void add_fadecolor(const color &_Color,float _Percent);
	void change_fadecolor(const size_t &_Index,const color &_Color,
		float _Percent);
	
	void remove_fadecolor(const size_t &_Index);
	void clear_fadecolors();
};

} //namespace lunar

#endif