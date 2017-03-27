/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: scaler.h
---------------------------------------------------
*/

#ifndef _SCALER_
#define _SCALER_

#include <vector>
#include "affector.h"

namespace lunar {

class scaler:public affector {

	friend affector_manager;

protected:

	struct _Scale_point {
	
	float _Size;
	float _Percent;
	};
	
	typedef std::vector<_Scale_point> _Scale_points;
	
	_Scale_points _Myscale_points;
	
	//Protected constructor
	scaler();
	
	//Protected copy constructor
	scaler(const scaler &_Scaler);
	
	//Protected virtual destructor
	virtual ~scaler();
	
	void copy(const scaler &_Scaler);
	
	virtual void affect(particle *_Particle,particle *_Particle_end,
		float _Time);

public:

	scaler &operator=(const scaler &_Scaler);
	
	void add_scalepoint(float _Size,float _Percent);
	
	void change_scalepoint(const size_t &_Index,float _Size,
		float _Percent);
	
	void remove_scalepoint(const size_t &_Index);
	void clear_scalepoints();
};

} //namespace lunar

#endif